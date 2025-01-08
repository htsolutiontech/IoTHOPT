#include "Utils_.h"

void setup()
{
  beginSerialCommunication();

  Wifi_Setup();

  NTP_Time_Setup();

  EEPROM_Setup();

  check_Reboot_Task();

  SHT30_Setup();

  Oled_Setup();

  SIM_Setup();

  GPS_Setup();

  GP2Y1014AU_Setup();

  HSTS016L_Setup();

  buzzer_Setup();

  displayActiveTime();
}

void loop()
{
  
  if (_updateLampStateInterval >= updateLampStateInterval)
  {
    update_Lamp_State();

    read_And_Check_Current();

    read_Temp_And_Humi();

    _updateLampStateInterval = 0;

  }

  oled_Animation_Task();

  displayActiveTime();

  read_Lat_And_Long();

  read_Air_Quality();

  read_Dust_Density();

  update_Data_To_Server();

  // print_All_Data();

  delay(20000);
}
