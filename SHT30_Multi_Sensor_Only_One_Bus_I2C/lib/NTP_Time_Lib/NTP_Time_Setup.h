#ifndef ___NTP_Time_Setup___
#define ___NTP_Time_Setup___

#include <time.h>
#include <stdio.h>
#include <Arduino.h>

// NTP parameters
extern const char *ntpServer1;
extern const char *ntpServer2;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

extern void set_Up_NTP_Time();

extern String get_Current_Time();

#endif // ___NTP_Time_Setup___
