#include "eepromReadStore.h"

int readByteFromEEPROM(int address)
{
  uint8_t val;
  uint8_t invertedval;
  val = EEPROM[address];
  invertedval = EEPROM[address+1];

  //Serial.print("READ address =");
  //Serial.print(address);
  //Serial.print(", result =");

  if (val == (~invertedval & 0xff)) {
    //Serial.println(val);
    return (val);
  } else {
    //Serial.println((int)-1);
    return (-1);
  }
  
}

void storeByteToEEPROM(uint8_t val, int address)
{
  EEPROM[address] = val;
  EEPROM[address+1] = ~val;
  //Serial.print("WRITE address =");
  //Serial.print(address);
  //Serial.print(", val =");
  //Serial.println(val);
}
