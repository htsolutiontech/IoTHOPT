#include <EEPROM_Write_Time.h>

uint64_t total_Seconds_OPR_Time = 0;
volatile boolean time_Flag = false;

volatile boolean _updateLampStatFlag = false;
uint64_t _updateLampStateInterval = 0;

volatile boolean last_time_Flag = false;

String OPR_Time;

uint32_t total_Switch_Machine_Count = 0;

hw_timer_t *timer = timerBegin(0, 80, true);

void displayActiveTime()
{
  unsigned long tempT = total_Seconds_OPR_Time;
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

void IRAM_ATTR count_OPR_Time()
{
  if (time_Flag)
  {
    total_Seconds_OPR_Time++;
  }

  if (_updateLampStatFlag)
  {
    _updateLampStateInterval++;
  }
  
}

void EEPROM_Time_Handler_Setup()
{
  timerAttachInterrupt(timer, count_OPR_Time, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void reset_Address_Eeprom() {
  uint16_t zero = 0;

  EEPROM.put(ADDRESS_STORE_OPR_TIME, zero); 
  EEPROM.put(ADDRESS_STORE_MACHINE_SWITCH_COUNT, zero); 
  EEPROM.commit();

}

void EEPROM_Setup()
{
  EEPROM.begin(512);

  // reset_Address_Eeprom();

  EEPROM.get(ADDRESS_STORE_OPR_TIME, total_Seconds_OPR_Time);
  EEPROM.get(ADDRESS_STORE_MACHINE_SWITCH_COUNT, total_Switch_Machine_Count);

  EEPROM_Time_Handler_Setup();

  displayActiveTime();
  
  _updateLampStatFlag = true;
}

void saveTime()
{
  EEPROM.put(ADDRESS_STORE_OPR_TIME, total_Seconds_OPR_Time);
  EEPROM.put(ADDRESS_STORE_MACHINE_SWITCH_COUNT, total_Switch_Machine_Count);
  EEPROM.commit();
  // Serial.println("___TIME DATA IS SAVED___");
}

void update_Lamp_State()
{
  if (time_Flag && !last_time_Flag)
  {
    total_Switch_Machine_Count++;
    // Serial.println("___THE LIGHT ON___");
    last_time_Flag = true;
  }

  if (!time_Flag && last_time_Flag)
  {
    saveTime();
    // Serial.println("___THE LIGHT OFF___");
    last_time_Flag = false;
  }
}