#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <SHT30.h>

#define SDA_1 21
#define SCL_1 22

#define SDA_2 27
#define SCL_2 26

SHT30 sensor1;
SHT30 sensor2;

void set_Up_Wifi();
void set_Up_NTP_Time();
String get_Current_Time();
void upload_Sensor_Data(float T1, float H1, float T2, float H2, float T3, float H3, String _Time);

// API URL
String URL = "https://iomt.hoangphucthanh.vn/";

// WiFi credentials
// const char *ssid = "HOPT_TẦNG 2";
// const char *password = "H@pt2020";
const char *ssid = "Minh Truong";
const char *password = "66661111";

// NTP para
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// Timing variables
uint32_t interval = 5000;
uint32_t lastMillis = 0;

float temperature1 = 0.0;
float humidity1 = 0.0;

float temperature2 = 0.0;
float humidity2 = 0.0;

float temperature3 = 0.0;
float humidity3 = 0.0;

void setup()
{
  Serial.begin(9600);

  Wire.begin(SDA_1, SCL_1);
  Wire1.begin(SDA_2, SCL_2);

  sensor1.begin(0x44, &Wire);
  sensor2.begin(0x44, &Wire1);

  set_Up_Wifi();

  set_Up_NTP_Time();
}

void loop()
{
  uint32_t currentMilss = millis();
  if (currentMilss - lastMillis >= interval)
  {
    lastMillis = currentMilss;

    String currentTime = get_Current_Time();

    temperature1 = sensor1.readTemperature();
    humidity1 = sensor1.readHumidity();

    temperature2 = sensor2.readTemperature();
    humidity2 = sensor2.readHumidity();

    Serial.print("Temperature1: ");
    Serial.print(temperature1);
    Serial.print(" °C, Humidity1: ");
    Serial.print(humidity1);
    Serial.print(" %, ");
    Serial.print("Temperature2: ");
    Serial.print(temperature2);
    Serial.print(" °C, Humidity2: ");
    Serial.print(humidity2);
    Serial.print(" %, ");
    Serial.println("Current Time: " + currentTime);

    upload_Sensor_Data(temperature1, humidity1, temperature2, humidity2, temperature3, humidity3, currentTime);
  }
}

void set_Up_Wifi()
{
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connect to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void set_Up_NTP_Time()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  while (!time(nullptr))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Time synced successfully");
}

String get_Current_Time()
{
  time_t now;
  struct tm timeinfo;

  time(&now);
  localtime_r(&now, &timeinfo);

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%d-%m-%y %H:%M:%S", &timeinfo);

  return String(timeString);
}

void upload_Sensor_Data(float T1, float H1, float T2, float H2, float T3, float H3, String _Time)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(URL);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature1=" + String(T1) + "&humidity1=" + String(H1) + "&temperature2=" + String(T2) 
                    + "&humidity2=" + String(H2) + "&temperature3=" + String(T3) + "&humidity3=" + String(H3) + "&time=" + _Time;

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println("Response from server: SUCCESS");
    }
    else
    {
      Serial.println("Error in sending data, HTTP code: " + String(httpResponseCode));
    }

    http.end();
  }
  else
  {
    Serial.println("Error in WiFi connection");
  }
}
