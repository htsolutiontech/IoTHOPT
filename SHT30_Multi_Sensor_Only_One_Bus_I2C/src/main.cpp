#include <Arduino.h>
#include <Varible_Config.h>

void setup()
{
  Serial.begin(9600);

  SHT30_Setup();

  GPS_Neo_Init();

  set_Up_Wifi();

  set_Up_NTP_Time();

  HSTS016L_Setup();

  delay(2000);
}

void loop()
{

  read_And_Check_Current();

  uint32_t currentMillis = millis();
  
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
