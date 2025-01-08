#ifndef ___GPS_Lib___
#define ___GPS_Lib___

#include <TinyGPS++.h>
#include <Wire.h>

#define GPS_RX 9
#define GPS_TX 10

extern TinyGPSPlus _gps;

extern bool checkNewData();

extern void GPS_Setup();

#endif // ___GPS_Lib___

