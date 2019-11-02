#include <Arduino.h>
#include <SPI.h>
#include "TPIC6C596SPI.h"

TPIC6C596SPI::TPIC6C596SPI(void)
{
}

void TPIC6C596SPI::begin(int numberOfCascadedChips, int rckPin, uint8_t bitorder)
{
    _numberOfCascadedChips = numberOfCascadedChips;
    _rckPin = rckPin;
    _bitorder = bitorder;
    //_doInvertData = doInvertData;

    pinMode(rckPin, OUTPUT);
    digitalWrite(rckPin, LOW);
    SPI.begin();
}

void TPIC6C596SPI::resetAndEnable(int ngPin, int nsrclrPin)
{
    pinMode(ngPin, OUTPUT);
    digitalWrite(ngPin, HIGH); // set all drain outputs to off
    pinMode(nsrclrPin, OUTPUT);
    digitalWrite(nsrclrPin, LOW); // clear all registers in connected devices
    digitalWrite(nsrclrPin, HIGH); // all registers are now cleared; drains are off
    digitalWrite(ngPin, LOW); // enable all drain outputs
}

void  TPIC6C596SPI::update(uint16_t data)
{
    SPI.beginTransaction (SPISettings (SPISPEED, _bitorder, SPI_MODE0));
    digitalWrite(_rckPin, LOW);
    //if (_doInvertData) {
    //    data = ~data;
    //}
    switch (_numberOfCascadedChips) {
        case ONETPIC6C596:  SPI.transfer((uint8_t)data);
                            break;
        case TWOTPIC6C596:  SPI.transfer16(data);
                            break;
        default:            break;
    }
    digitalWrite(_rckPin, HIGH); // rising edge clocks register bank
    SPI.endTransaction ();
}


