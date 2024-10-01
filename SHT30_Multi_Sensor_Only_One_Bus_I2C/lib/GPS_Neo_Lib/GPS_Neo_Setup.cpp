#include <GPS_Neo_Setup.h>

HardwareSerial mySerial(2);

TinyGPSPlus _gps;

void GPS_Neo_Init() {
    mySerial.begin(9600, SERIAL_8N1, RX, TX);
}


bool checkNewData() {
  bool newData = false;
  unsigned long start = millis();
  while (millis() - start < 1000) {
    while (mySerial.available()) {
      if (_gps.encode(mySerial.read())) {
        newData = true;
      }
    }
  }
  return newData;
}