#ifndef __Utils___
#define __Utils___

#include <Arduino.h>
#include "Oled_Lib.h"
#include "Buzzer_Lib.h"
#include "MQ135.h"
#include "SHT30.h"
#include "GPS_Lib.h"
#include "Wifi_Lib.h"
#include "NTP_Time_Setup.h"
#include "HSTS016L_Lib.h"
#include "GP2Y1014AU.h"
#include "EEPROM_Write_Time.h"
#include "Update_Server_Lib.h"
#include "A7680C_SIM_Lib.h"

/* millis innit */
extern uint32_t lastSensorReadTime;
extern uint32_t sensorReadInterval;

extern uint16_t updateLampStateInterval;

extern uint8_t countOledAnimation;

extern void oled_Animation_Task();

/* Serial Begin*/
extern void beginSerialCommunication();

/* Check store data to EEPROM */
#define LED_PIN 2

extern void blynk_Led (uint8_t ledPin, uint8_t blinkCount, uint16_t blinkInterval);

extern void check_Reboot_Task();


/* GP2Y1014AU Sensor */
#define measurePin 26
#define ledPin 25

extern GP2Y1010AU0F dustSensor;

extern float dustDensity;

extern void GP2Y1014AU_Setup();

extern void read_Dust_Density();

/* HSTS016L Sensor */
#define VOUT_PIN 32
#define VREF_PIN 33

#define NUM_SAMPLES 5000
#define SAMPLE_INTERVAL 200

#define CURRENT_OFFSET 10.1

#define OPR_CURRENT_THRESHOLD 15

#define WARNING_POWER_THRESHOLD 255
// #define WARNING_POWER_THRESHOLD 220

#define OPR_VOLTAGE 13.2

extern HSTS016L_Config_t HST016L_SENSOR;

extern float nowCurrent;
extern float nowPower;

extern bool buzzerState;

extern void HSTS016L_Setup();

extern void read_And_Check_Current();

/* MQ135 Sensor */
#define MQ135_ANA_PIN 39

extern MQ135 MQ135_SENSOR;

extern float PPM_Value;

extern void read_Air_Quality();

/* SHT30 Sensor*/
#define SHT30_I2C_SDA 27
#define SHT30_I2C_SCL 14


#define WARNING_TEMPERATURE_THRESHOLD 40

extern SHT30 SHT30_SENSOR;

extern float temperature;
extern float humidity;

extern bool tempState;

extern void SHT30_Setup();

extern void read_Temp_And_Humi();

/* Oled */
#define OLED_I2C_SDA 21
#define OLED_I2C_SCL 22

extern void Oled_Setup();

/* Module GPS*/
extern double latitude;
extern double longitude;

extern void read_Lat_And_Long();


/* NTP_Time */
extern String currentTime;


/* Print Function */
extern void print_All_Data();


/* Update data to Server */

extern void connectToWiFi();

extern void update_Data_To_Server();



#endif /* __Utils___ */