#ifndef EEPROMREADSTORE_H
#define EEPROMREADSTORE_H
#endif

#include <Arduino.h>
#include <EEPROM.h>

int readByteFromEEPROM(int address);
void storeByteToEEPROM(uint8_t val, int address);