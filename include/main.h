#include <Arduino.h>
#include "eepromreadstore.h"
#include "RelayBank.h"

const int RelayEnergizingTimeMilliseconds = 5; // Minimum Duration of current pulse for correct latching
const int RCK_M_CPU = 2; // Register Clock Pin of TPIC6c596 for LC-CL-Selection
const int RCK_L_CPU = 3; // Register Clock Pin of TPIC6c596 for L-Section
const int RCK_C_CPU = 4; // Register Clock Pin of TPIC6c596 for C-Section
const int NSRCLR_CPU = 5; // Serial Register Clear Pin for all TPIC6c596
const int NG_CPU = 6; // Enable Pin for all TPIC6c596
const int MOSI_CPU = 11; // SPI Data Output Pin 
const int MISO_CPU = 12; // SPI Data Input Pin (not used)
const int SCK_CPU = 13;  // SPI Clock Pin

const int NumberOfBands = 10;
const int BAddress = 0; // each 8-bit-parameter is stored twice: non-inverted at addr and inverted at addr+1
const int MAddress = 2;
const int LAddress = MAddress + 2 * NumberOfBands;
const int CAddress = LAddress + 2 * NumberOfBands;
const int DefaultBand = 0;
const char NOP = '\0';

int band = 0;
int readresult;
bool DoPrintCmdResponse = false;

RelayBank mbank;
RelayBank lbank;
RelayBank cbank;

void printUsageMessage(void);
void printCmdResponse(int band, RelayBank* mbank, RelayBank* lbank, RelayBank* cbank);
int readBand(int bAddress, int defaultBand);
void storeBand(int band, int bAddress);
