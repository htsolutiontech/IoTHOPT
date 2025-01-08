#include <GPS_Lib.h>

HardwareSerial GPS_Serial(2);

TinyGPSPlus _gps;

void GPS_Setup() {
    GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}


bool checkNewData() {
  bool newData = false;
  unsigned long start = millis();
  while (millis() - start < 1000) {
    while (GPS_Serial.available()) {
      if (_gps.encode(GPS_Serial.read())) {
        newData = true;
      }
    }
  }
  return newData;
}