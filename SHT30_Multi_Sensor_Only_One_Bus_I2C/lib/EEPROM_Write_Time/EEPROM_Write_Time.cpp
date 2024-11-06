#include <EEPROM_Write_Time.h>

uint64_t t_Store_OPR_Time = 0;
volatile boolean time_Flag = false;
volatile boolean last_time_Flag = false;

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

  OPR_Time = (h < 10 ? "0" : "") + String(h) + ":" +
             (m < 10 ? "0" : "") + String(m) + ":" +
             (s < 10 ? "0" : "") + String(s);

  // char OPR_TimeString[20];
  // sprintf(OPR_TimeString, "%02lu:%02lu:%02lu", h, m, s); // Định dạng OPR_Time thành HH:MM:SS
  // OPR_Time = String(OPR_TimeString);
}

void EEPROM_Init()
{
  EEPROM.begin(512);
  EEPROM.get(0, t_Store_OPR_Time);
  EEPROM.get(COUNT_ON_OFF_LIGHT_ADDRESS, on_Off_Light_Count);
}

void IRAM_ATTR count_OPR_Time()
{
  if (time_Flag)
  {
    t_Store_OPR_Time++;
  }
}

void EEPROMTimeHandler_Init()
{
  timerAttachInterrupt(timer, count_OPR_Time, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void saveTime()
{
  EEPROM.put(0, t_Store_OPR_Time);
  EEPROM.put(COUNT_ON_OFF_LIGHT_ADDRESS, on_Off_Light_Count);
  EEPROM.commit();
  Serial.println("___TIME DATA IS SAVED___");
}

void update_Light_State()
{
  if (time_Flag && !last_time_Flag)
  {
    on_Off_Light_Count++;
    Serial.println("___THE LIGHT ON___");
    last_time_Flag = true;
  }

  if (!time_Flag && last_time_Flag)
  {
    saveTime();
    Serial.println("___THE LIGHT OFF___");
    last_time_Flag = false;
  }
}