#ifndef ___GPS_Neo_Setup___
#define ___GPS_Neo_Setup___

#include <TinyGPS++.h>
#include <Wire.h>

#define RX 16
#define TX 17

extern TinyGPSPlus _gps;

extern bool checkNewData();

extern void GPS_Neo_Init();

#endif // ___GPS_Neo_Setup___

