#include "Utils_.h"

/* millis innit */
uint32_t lastSensorReadTime = 0;
uint32_t sensorReadInterval = 15000;

uint16_t updateLampStateInterval = 1;

uint8_t countOledAnimation = 0;

void oled_Animation_Task()
{
  countOledAnimation++;

  if (countOledAnimation >= 5)
  {
    oled_Animation();

    countOledAnimation = 0;
  }
}

/* Serial Begin */
void beginSerialCommunication()
{
  Serial.begin(115200);
}

/* Check store data to EEPROM */
void blynk_Led(uint8_t _ledPin, uint8_t blinkCount, uint16_t blinkInterval)
{
  uint32_t lastMillis = 0;
  boolean ledState = LOW;
  uint8_t t_blink = 0;

  while (t_blink < blinkCount)
  {
    uint32_t nowMillis = millis();

    if (nowMillis - lastMillis >= blinkInterval)
    {
      lastMillis = nowMillis;

      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(_ledPin, ledState);

      if (ledState == LOW)
        t_blink++;
    }
  }

  digitalWrite(_ledPin, LOW);
}

void check_Reboot_Task()
{
  pinMode(LED_PIN, OUTPUT);

  blynk_Led(LED_PIN, 3, 300);
}

/* HSTS016L Sensor */
HSTS016L_Config_t HST016L_SENSOR;

float nowCurrent = 0.0;
float nowPower = 0.0;

void HSTS016L_Setup()
{
  HSTS016L_Init(&HST016L_SENSOR, VOUT_PIN, VREF_PIN, DEFAULT_R, DEFAULT_Q);

  uint16_t initialCurrent = analogRead(VOUT_PIN);
  uint16_t initialCalib = analogRead(VREF_PIN);

  HSTS016L_InitKalmanState(&HST016L_SENSOR, initialCurrent, initialCalib);
}

bool buzzerState = false; 

void read_And_Check_Current()
{
  nowCurrent = HSTS016L_ReadDCCurrent(&HST016L_SENSOR, NUM_SAMPLES, SAMPLE_INTERVAL, CURRENT_OFFSET);
  nowPower = nowCurrent * OPR_VOLTAGE;

  if (nowCurrent < OPR_CURRENT_THRESHOLD)
  {
    nowCurrent = 0.0;
    nowPower = 0.0;
    if (buzzerState)
    {
      buzzer_OFF();
      buzzerState = false;
    }
  }
  else if (nowPower >= WARNING_POWER_THRESHOLD)
  {
    if (!buzzerState)
    {
      sendSMS(NUMBER_PHONE, POWER_WARNING);
      delay(10);
      sendSMS(DAT_SDT, POWER_WARNING);
      delay(10);
      sendSMS(ANH_THUONG, POWER_WARNING);
      delay(10);
      sendSMS(ANH_NHAN_NHO, POWER_WARNING);
      delay(10);
      sendSMS(PHUONG_SDT, POWER_WARNING);
      buzzer_Warning();
      buzzerState = true;
    }
  }
  else
  {
    if (buzzerState)
    {
      buzzer_OFF();
      buzzerState = false;
    }
  }

  time_Flag = (nowCurrent >= OPR_CURRENT_THRESHOLD);
}


/* SHT30 Sensor*/
SHT30 SHT30_SENSOR;

float temperature = 0.0;
float humidity = 0.0;

bool tempState = false;

void SHT30_Setup()
{
  SHT30_begin(&SHT30_SENSOR, SHT30_I2C_SDA, SHT30_I2C_SCL);
  SHT30_init(&SHT30_SENSOR, 0x44);
}

void read_Temp_And_Humi()
{
  temperature = SHT30_readTemperature(&SHT30_SENSOR);
  humidity = SHT30_readHumidity(&SHT30_SENSOR);

  if (temperature >= WARNING_TEMPERATURE_THRESHOLD)
  {
    if (!tempState)
    {
      sendSMS(NUMBER_PHONE, TEMP_WARNING);
      delay(10);
      sendSMS(DAT_SDT, TEMP_WARNING);
      delay(10);
      sendSMS(ANH_THUONG, TEMP_WARNING);
      delay(10);
      sendSMS(ANH_NHAN_NHO, TEMP_WARNING);
      delay(10);
      sendSMS(PHUONG_SDT, TEMP_WARNING);
      buzzer_Warning();
      tempState = true;
    }
  }

  else
  {
    buzzer_OFF();
    tempState = false;
  }
}

/* MQ135 Sensor */
MQ135 MQ135_SENSOR = MQ135(MQ135_ANA_PIN);

float PPM_Value = 0.0;

void read_Air_Quality()
{
  PPM_Value = MQ135_SENSOR.getCorrectedPPM(temperature, humidity);
}


/* Oled */
void Oled_Setup()
{
  OLED_I2C.begin(OLED_I2C_SDA, OLED_I2C_SCL);

  oled_Init();
}

/* Module GPS*/

double latitude = 0.0;
double longitude = 0.0;

void read_Lat_And_Long()
{
  if (checkNewData())
  {
    latitude = _gps.location.lat();
    longitude = _gps.location.lng();
  }
}

/* Print Function */
void print_All_Data()
{

  Serial.println("Updated Time: " + currentTime);

  Serial.print(F("Now Current: "));
  Serial.print(nowCurrent);
  Serial.print(" A, ");

  Serial.print(F("Now Power: "));
  Serial.print(nowPower);
  Serial.print(" W, ");

  Serial.print("Total Switch Machine: ");
  Serial.print(total_Switch_Machine_Count);

  Serial.println(", Machine OPR Time: " + OPR_Time);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %, ");

  Serial.print("Air Quality: ");
  Serial.print(PPM_Value);
  Serial.print(" PPM, ");

  Serial.print("Dust Density = ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");

  Serial.print(F("Latitude: "));
  Serial.print(latitude, 6);
  Serial.print(F(", Longitude: "));
  Serial.println(longitude, 6);

  Serial.println();
}

/* GP2Y1014AU Sensor */
GP2Y1010AU0F dustSensor(ledPin, measurePin);

float dustDensity = 0.0;

void GP2Y1014AU_Setup()
{
  dustSensor.begin();
}

void read_Dust_Density()
{
  WiFi.mode(WIFI_OFF);
  delay(10);

  dustDensity = dustSensor.read();
}

/* Update data to Server */
/* NTP_Time */

String currentTime;

void connectToWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000)
    {
      delay(100); // Retry for 5 seconds
      // Serial.print(".");
    }

    // if (WiFi.status() == WL_CONNECTED)
    // {
    //   // Serial.println("\nConnected to WiFi");
    // }
    // else
    // {
    //   // Serial.println("\nFailed to connect to WiFi");
    // }
  }
}

void update_Data_To_Server()
{
  connectToWiFi(); // Kiểm tra và kết nối WiFi

  // Serial.print("Check Heap Map: ");
  // Serial.println(ESP.getFreeHeap());

  // if (WiFi.status() != WL_CONNECTED)
  // {
  //   // Serial.println("Skipping data upload: No WiFi connection.");
  //   return;
  // }

  currentTime = get_Current_Time();

  upload_Data(temperature, humidity, PPM_Value, dustDensity, nowCurrent, nowPower,
              latitude, longitude, time_Flag, total_Switch_Machine_Count, OPR_Time, currentTime);

  if (httpResponseCode > 0)
  {
    // Serial.println("Data uploaded successfully!");
    blynk_Led(LED_PIN, 3, 300); // Response from server: SUCCESS
  }
  else
  {
    // Serial.println("Failed to upload data to server. HTTP response code: " + String(httpResponseCode));
  }
}
