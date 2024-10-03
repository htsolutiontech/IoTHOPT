#include <EEPROM_Write_Time.h>

uint32_t t_Store_OPR_Time = 0;
bool isSaved = false;
bool isStarted = false;
volatile boolean time_Flag = false;
String OPR_Time;

uint32_t on_Off_Light_Count = 0;
const uint8_t COUNT_ON_OFF_LIGHT_ADDRESS = sizeof(t_Store_OPR_Time);

hw_timer_t *timer = timerBegin(0, 80, true);

void displayActiveTime()
{
  unsigned long tempT = t_Store_OPR_Time;
  unsigned long s = tempT % 60;
  tempT /= 60;
  unsigned long m = tempT % 60;
  unsigned long h = tempT / 60;

  OPR_Time = String(h) + ":" + String(m) + ":" + String(s);

  // Serial.printf("OPRed TIME: %02lu:%02lu:%02lu\n", h, m, s);
  // Serial.println("OPRed TIME: " + OPR_Time);
}

void EEPROM_Setup()
{
  EEPROM.begin(512);
  EEPROM.get(0, t_Store_OPR_Time);
  EEPROM.get(COUNT_ON_OFF_LIGHT_ADDRESS, on_Off_Light_Count);
}

void saveTime()
{
  EEPROM.put(0, t_Store_OPR_Time);
  EEPROM.put(COUNT_ON_OFF_LIGHT_ADDRESS, on_Off_Light_Count);
  EEPROM.commit();
  isSaved = true;
  isStarted = false;
}

void IRAM_ATTR count_OPR_Time()
{
  if (time_Flag)
  {
    t_Store_OPR_Time++;
  }
}

void Handler_EEP_Setup()
{
  timerAttachInterrupt(timer, count_OPR_Time, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void update_Light_State()
{
  if (time_Flag && !isStarted)
  {
    isStarted = true;
    on_Off_Light_Count ++;
    Serial.println("___THE LIGHT ON___");
  }

  if (!time_Flag && !isSaved)
  {
    saveTime();
    Serial.println("___THE LIGHT OFF___");
    Serial.println("===================");
    Serial.println("==============================");
    Serial.println("============================================");
  }
}