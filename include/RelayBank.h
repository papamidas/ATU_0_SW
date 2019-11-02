// LCBank
// Es wird angenommen, dass 1 oder mehrere TPIC6C596 zum Ansteuern von bistabilen Relais
// mit jeweils 2 Wicklungen verwendet werden, zum Beispiel HONGFA HFD2-L2 und dass diese
// so angeschlossen sind:
// Drain0: Reset L oder C
// Drain1: Set L oder C
// Drain2: Reset 2L oder 2C
// Drain3: Set 2L oder 2C
// ...
// Drain14: Reset 128L oder 128C
// Drain15: Set 128L oder 128C

#ifndef RelayBank_h
#define RelayBank_h
#endif

#include "Arduino.h"
#include "TPIC6C596SPI.h"

const uint8_t BANKRESETVAL = B0100000 + B00010000 + B00000100 + B00000001;

class RelayBank : public TPIC6C596SPI {
   public:
    RelayBank();
    void    begin(tpic6c596no_t numberOfCascadedChips, int rckPin, uint8_t bitorder,
                 bool doInvertData, int eepromAddress, int band, int energizingTimeMilliseconds,
                 float stepSize, String stepPhysicalUnit);
    void    reset(void);
    void    setBandAndUpdate(int band);
    void    setAbsolute(uint8_t binaryValue);
    void    setRelative(int increment);
    float   getValue(void);
    String  getValueString(void);

private:
    int 	_eepromAddress;
    int     _band;
    int     _energizingTimeMilliseconds;
    float   _stepSize;
    String  _stepUnit;
    uint8_t _binaryValue;
    int     _registerWord;
    bool	_doInvertData;

};