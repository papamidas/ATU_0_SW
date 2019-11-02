#include <Arduino.h>
#include <SPI.h>
#include "RelayBank.h"
#include "eepromReadStore.h"

RelayBank::RelayBank(void)
{
}

void RelayBank::begin(tpic6c596no_t numberOfCascadedChips, int rckPin, uint8_t bitorder,
                        bool doInvertData, int eepromAddress, int band, int energizingTimeMilliseconds,
                        float stepSize, String stepUnit)
{
    TPIC6C596SPI::begin(numberOfCascadedChips, rckPin, bitorder);
    _eepromAddress = eepromAddress;
    _energizingTimeMilliseconds = energizingTimeMilliseconds;
    _stepSize = stepSize;
    _stepUnit = stepUnit;
    _doInvertData = doInvertData;

    reset();
    setBandAndUpdate(band);
}

void RelayBank::reset(void)
{
    _registerWord = (BANKRESETVAL << 8) + BANKRESETVAL;
    update(_registerWord);
    _binaryValue = 0;
}

void RelayBank::setBandAndUpdate(int band)
{
    _band = band;
    int bv;
    if ((bv=readByteFromEEPROM(_eepromAddress + 2 * _band)) >= 0) {
        _binaryValue = bv;
        //Serial.print(", bv =");
        //Serial.print(bv);
    } else
    {
        _binaryValue = 0;
    }
    //Serial.print("read address =");
    //Serial.print(_eepromAddress + 2 * _band);
    //Serial.print(", _binaryvalue =");
    //Serial.print(_binaryValue);
    //Serial.print(", _band =");
    //Serial.println(_band);
    setAbsolute(_binaryValue);
}

void RelayBank::setAbsolute(uint8_t binaryValue)
{
    // wenn bit n == 0 und _bit n == 0: do nothing or reset bit n
    // wenn bit n == 0 und _bit n == 1: reset bit n
    // wenn bit n == 1 und _bit n == 0: set bit n
    // wenn bit n == 1 und _bit n == 1: do nothing or set bit n
    uint8_t bv;
    int bitpos = 0;
    _registerWord = 0;
    //Serial.print(", _binaryvalue =");
    //Serial.print(_binaryValue);
    _binaryValue = binaryValue;
    if(_doInvertData) {
        bv = ~binaryValue;
    } else {
        bv = binaryValue;
    }
    for( int i=0; i<8; i++) {
        if(bv & (uint8_t)(1<<i)) {
            _registerWord |= (1<<bitpos);
        } else {
            _registerWord |= (1<<(bitpos+1));
        }
        bitpos+=2;
    }
    update(_registerWord);  // Set- und Reset-Wicklungen bestromen
//    Serial.print("update(");
//    Serial.print(_registerWord);
//    Serial.println(")");
    delay(_energizingTimeMilliseconds);
    update(0);              // Strom abschalten
    storeByteToEEPROM(_binaryValue, _eepromAddress + 2 * _band);
    //Serial.print("write address =");
    //Serial.print(_eepromAddress + 2 * _band);
    //Serial.print(", _band =");
    //Serial.println(_band);
 }

void RelayBank::setRelative(int increment)
{
    int nextBinaryValue = _binaryValue + increment;
     switch (_numberOfCascadedChips) {
        case ONETPIC6C596:  nextBinaryValue = constrain(nextBinaryValue, 0, 15);
                            break;
        case TWOTPIC6C596:  nextBinaryValue = constrain(nextBinaryValue, 0, 255);
                            break;
        default:            nextBinaryValue = _binaryValue;
                            break;
    }
    _binaryValue = (uint8_t) nextBinaryValue;
    setAbsolute(_binaryValue);
    //Serial.print("setAbsolute(");
    //Serial.print(_binaryValue);
    //Serial.println(")");
}

float RelayBank::getValue(void)
{
    return(_stepSize * _binaryValue);
}

String RelayBank::getValueString(void)
{
    String valueString = String(getValue(), 2);
    return(valueString + _stepUnit);
}


