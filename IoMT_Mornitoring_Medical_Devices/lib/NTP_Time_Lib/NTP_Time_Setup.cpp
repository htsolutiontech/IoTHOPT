#include <NTP_Time_Setup.h>

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

void NTP_Time_Setup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  while (!time(nullptr))
  {
    delay(300);
    // Serial.print(".");
  }
  // Serial.println("Time synced successfully");
  // Serial.println();
}

String get_Current_Time()
{
  time_t now;
  struct tm timeinfo;

  time(&now);
  localtime_r(&now, &timeinfo);

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%y-%m-%d %H:%M:%S", &timeinfo);

  return String(timeString);
}