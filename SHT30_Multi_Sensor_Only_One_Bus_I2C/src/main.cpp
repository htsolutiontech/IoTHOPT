#include <Arduino.h>
#include <Varible_Config.h>

void setup()
{
  beginSerialCommunication();

  Wifi_Init();

  NTP_Time_Init();

  EEPROM_Init();

  EEPROMTimeHandler_Init();

  GPS_Neo_Init();

  HSTS016L_Init();

  SHT30_Init();

  displayActiveTime();
}

void loop()
{

  uint16_t currentMillis_2 = millis();
  if (currentMillis_2 - lastMillis_2 >= interval_2)
  {
    lastMillis_2 = currentMillis_2;

    read_And_Check_Current(); 

    update_Light_State();
    
  }


  uint16_t currentMillis = millis();
  if (currentMillis - lastMillis >= interval)
  {
    lastMillis = currentMillis;

    if (time_Flag)
    {
      saveTime();
    }
    
    read_Lat_And_Long();

    read_Real_Time();

    read_Temp_And_Humi();

    print_Data_Var();

    update_Data_To_Server();
    
  }

}