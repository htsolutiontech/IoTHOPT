#ifndef ___NTP_Time_Lib___
#define ___NTP_Time_Lib___

#include <time.h>
#include <stdio.h>
#include <Arduino.h>

// NTP parameters
extern const char *ntpServer1;
extern const char *ntpServer2;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

extern void NTP_Time_Setup();

extern String get_Current_Time();

#endif // ___NTP_Time_Lib___
