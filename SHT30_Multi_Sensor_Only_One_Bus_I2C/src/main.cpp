#include <Arduino.h>
#include <Varible_Config.h>

void setup()
{
  Serial.begin(9600);

  EEPROM_Setup();
  Handler_EEP_Setup();

  SHT30_Setup();

  GPS_Neo_Init();

  set_Up_Wifi();

  set_Up_NTP_Time();

  HSTS016L_Setup();

  displayActiveTime();
}

void loop()
{
  read_And_Check_Current();

  update_Light_State();

  uint16_t currentMillis = millis();
  if (currentMillis - lastMillis >= interval)
  {
    lastMillis = currentMillis;

    read_Lat_And_Long();

    read_Real_Time();

    read_Temp_And_Humi();

    print_Data_Var();

    update_Data_To_Server();
    
  }
}
