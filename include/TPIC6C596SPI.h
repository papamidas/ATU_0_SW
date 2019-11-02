#ifndef TPIC6C596SPI_h
#define TPIC6C596SPI_h
#endif

#include "Arduino.h"

typedef int tpic6c596no_t;
const tpic6c596no_t ONETPIC6C596 = 1;
const tpic6c596no_t TWOTPIC6C596 = 2;
const uint32_t SPISPEED = 8000000;
const bool DOINVERTDATA = true;
const bool DONOTINVERTDATA = false;

class TPIC6C596SPI 
{
public:
    TPIC6C596SPI();
    void begin(tpic6c596no_t numberOfCascadedChips, int rckPin, uint8_t bitorder);
    void resetAndEnable(int ngPin, int nsrclrPin);
    void update(uint16_t data);
 
protected:
    int     _numberOfCascadedChips;
    int     _rckPin;
    int     _sckfrequency;
    uint8_t _bitorder;

};