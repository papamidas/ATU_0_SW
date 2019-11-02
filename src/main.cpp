#include "main.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(NSRCLR_CPU, OUTPUT);
  digitalWrite(NSRCLR_CPU, HIGH);
  pinMode(NG_CPU, OUTPUT);
  digitalWrite(NG_CPU, LOW); //chip enable oder so
  band = readBand(BAddress, DefaultBand);
  mbank.begin(ONETPIC6C596, RCK_M_CPU, MSBFIRST, DONOTINVERTDATA, MAddress, band, RelayEnergizingTimeMilliseconds, 1.0, "");
  lbank.begin(TWOTPIC6C596, RCK_L_CPU, MSBFIRST, DOINVERTDATA, LAddress, band, RelayEnergizingTimeMilliseconds, 0.06, "µH");
  cbank.begin(TWOTPIC6C596, RCK_C_CPU, LSBFIRST, DONOTINVERTDATA, CAddress, band, RelayEnergizingTimeMilliseconds, 0.01, "nF");
  printCmdResponse(band, &mbank, &lbank, &cbank);
}

void loop() {
  // put your main code here, to run repeatedly:
  char inChar = NOP;
  if (Serial.available()) {
    inChar = (char)Serial.read();
  }

  DoPrintCmdResponse = true;
  switch(inChar) {
    case NOP:   DoPrintCmdResponse = false;
                break;
    case '0':         // Select Band 0..9
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': //Serial.print(F("Band select command: "));
              //Serial.println(inChar);
              if (band != inChar - '0') {
                band = inChar - '0';
                storeBand(band, BAddress);
              };
              mbank.setBandAndUpdate(band);
              lbank.setBandAndUpdate(band);
              cbank.setBandAndUpdate(band);
              break;
    case 'c':
    case 'C':         // Select CL match
              //Serial.print(F("CL match select command: "));
              //Serial.println(inChar);
              mbank.setAbsolute(1);
              break;
    case 'l':
    case 'L':         // Select LC match
              //Serial.print(F("LC match select command: "));
              //Serial.println(inChar);
              mbank.setAbsolute(2);
              break;
    case 'u':         // increase L by one step
              //Serial.print(F("increase L by 1 command: "));
              //Serial.println(inChar);
              lbank.setRelative(1);
              break;
    case 'U':         // increase L by ten steps
              //Serial.print(F("increase L by 10 command: "));
              //Serial.println(inChar);
              lbank.setRelative(10);
              break;
    case 'd':         // decrease L by one step
              //Serial.print(F("decrease L by 1 command: "));
              //Serial.println(inChar);
              lbank.setRelative(-1);
              break;
    case 'D':         // decrease L by ten steps
              //Serial.print(F("decrease L by 10 command: "));
              //Serial.println(inChar);
              lbank.setRelative(-10);
              break;
    case 'i':         // increase C by one step
              //Serial.print(F("increase C by 1 command: "));
              //Serial.println(inChar);
              cbank.setRelative(1);
              break;
    case 'I':         // increase C by 10 steps
              //Serial.print(F("increase C by 10 command: "));
              //Serial.println(inChar);
              cbank.setRelative(10);
              break;
    case 'r':         // decrease C by one step
              //Serial.print(F("decrease C by 1 command: "));
              //Serial.println(inChar);
              cbank.setRelative(-1);
              break;
    case 'R':         // decrease C by 10 steps
              //Serial.print(F("decrease C by 10 command: "));
              //Serial.println(inChar);
              cbank.setRelative(-10);
              break;
    case '?': DoPrintCmdResponse = false;
              printUsageMessage();
              break;         
    default:  DoPrintCmdResponse = false;
  }
  if (DoPrintCmdResponse)
      printCmdResponse(band, &mbank, &lbank, &cbank);
  delay(10);
}

int readBand(int bAddress, int defaultBand)
{
    int band=0;
    int eepromReadResult = readByteFromEEPROM(bAddress);  //try to read stored Band variable
    if (eepromReadResult < 0) {
      band = defaultBand;
      storeByteToEEPROM(band, bAddress);
    }
    else {
      band = eepromReadResult & 0xff;
    }
    return(band);
}

void storeBand(int band, int bAddress)
{
      storeByteToEEPROM(band, bAddress);
}

void printCmdResponse(int band, RelayBank* mbank, RelayBank* lbank, RelayBank* cbank)
{
  Serial.print("Band ");
  Serial.print(band);
  Serial.print(": ");
  if (mbank->getValue() < 1.5) {
    Serial.print("CL-match, ");
  } else {
    Serial.print("LC-match, ");
  }
  Serial.print(lbank->getValueString());
  Serial.print(", ");
  Serial.println(cbank->getValueString());
}

void printUsageMessage(void)
{
  Serial.println("Usage: ");
  Serial.println("(0),(1),...,(9): Select Band");
  Serial.println("(C): Select CL match");
  Serial.println("(L): Select LC match");
  Serial.println("(u): increase L");
  Serial.println("(U): increase L by 10 steps");
  Serial.println("(d): decrease L");
  Serial.println("(D): decrease L by 10 steps");
  Serial.println("(i): increase C");
  Serial.println("(I): increase C by 10 steps");
  Serial.println("(r): decrease C");
  Serial.println("(R): decrease C by 10 steps");
}

