#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <SHT30.h>
#include <TinyGPS++.h>
#include <WCS1800.h>
#include <SimpleKalmanFilter.h>

#define RX 16
#define TX 17

#define I2C_SDA 21
#define I2C_SCL 22

// #define SDA_2 27
// #define SCL_2 26

#define WCS1800_PIN 36

#define _numReadings 10

SHT30 _sensor;
// SHT30 sensor2;

TinyGPSPlus _gps;

SimpleKalmanFilter _kalmanFilter(1, 1, 0.01);

WCS1800 wcs(WCS1800_PIN);

HardwareSerial mySerial(2);

void set_Up_Wifi();
void set_Up_NTP_Time();
String get_Current_Time();
void upload_Data(float T1, float H1, float T2, float H2, float T3, float H3, float current, double lat, double lng,  boolean timFlag, String _Time);

void updateSerial();
bool checkNewData();

void selectChannel(uint8_t bus);
float filteredCurrent(float dcCurrent, int filterNum);

// API URL
String URL = "https://iomt.hoangphucthanh.vn/";

// WiFi credentials
const char *ssid = "HOPT_TẦNG 2";
const char *password = "H@pt2020";
// const char *ssid = "Minh Truong";
// const char *password = "66661111";

// const char *ssid = "HOPT-VNPT_2.4G";
// const char *password = "H@pt2020";

// NTP parameters
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

double latitude = 0.0;
double longitude = 0.0;

double nowCurrent = 0.0;

volatile boolean time_Flag = false;


void setup() {
  Serial.begin(9600);

  Wire.begin(I2C_SDA, I2C_SCL);
  
  _sensor.begin(0x44, &Wire);

  mySerial.begin(9600, SERIAL_8N1, RX, TX);

  wcs.begin();

  set_Up_Wifi();
  set_Up_NTP_Time();

  delay(2000);
}

void loop() {
  uint32_t currentMillis = millis();
  boolean newData = false;
  if (currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;

    if (checkNewData()) {
      latitude = _gps.location.lat();
      longitude = _gps.location.lng();
    }

    String currentTime = get_Current_Time();
    wcs.readData();

    // Simulate sensor data
    // selectChannel(2);
    // temperature1 = _sensor.readTemperature();
    // humidity1 = _sensor.readHumidity();

    temperature1 = random(10, 40);
    humidity1 = random(30, 100);


    // selectChannel(3);
    // temperature2 = _sensor.readTemperature();
    // humidity2 = _sensor.readHumidity();

    temperature2 = random(10, 40);
    humidity2 = random(30, 100);

    // selectChannel(4);
    // temperature3 = _sensor.readTemperature();
    // humidity3 = _sensor.readHumidity();

    temperature3 = random(10, 40);
    humidity3 = random(30, 100);

    // nowCurrent = wcs.getCurrent();
    // nowCurrent = filteredCurrent(nowCurrent, 1);
    nowCurrent = random(5, 30);
    if (nowCurrent >= 10)
    {
      time_Flag = true;
    }
    else time_Flag = false;
    

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
    Serial.print("Temperature3: ");
    Serial.print(temperature3);
    Serial.print(" °C, Humidity3: ");
    Serial.print(humidity3);
    Serial.println(" %, ");

    Serial.print(F("Current: "));
    Serial.print(nowCurrent);
    Serial.print(" A");
    Serial.print(F(", Latitude: "));
    Serial.print(latitude, 6);
    Serial.print(F(", Longitude: "));
    Serial.print(longitude, 6);
    Serial.print(", Time Flag: ");
    Serial.print(time_Flag);
    Serial.println(", Current Time: " + currentTime);
    
    upload_Data(temperature1, humidity1, temperature2, humidity2, temperature3, humidity3, nowCurrent, latitude, longitude, time_Flag, currentTime);
  }
}

void set_Up_Wifi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connect to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void set_Up_NTP_Time() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  while (!time(nullptr)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Time synced successfully");
}

String get_Current_Time() {
  time_t now;
  struct tm timeinfo;

  time(&now);
  localtime_r(&now, &timeinfo);

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%y-%m-%d %H:%M:%S", &timeinfo);

  return String(timeString);
}

void upload_Data(float T1, float H1, float T2, float H2, float T3, float H3, float current, double lat, double lng, boolean timFlag ,String _Time) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature1=" + String(T1) + "&humidityAir1=" + String(H1) + "&temperature2=" + String(T2) 
                    + "&humidityAir2=" + String(H2) + "&temperature3=" + String(T3) + "&humidityAir3=" + String(H3)
                    + "&lon=" + String(lng, 6) + "&lat=" + String(lat, 6)
                    + "&timeFlag=" + String(timFlag) 
                    + "&currentValue=" + String(current) + "&time=" + _Time;

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      // Serial.println(response);
      Serial.println("Response from server: SUCCESS");
    } else {
      Serial.println("Error in sending data, HTTP code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

bool checkNewData() {
  bool newData = false;
  unsigned long start = millis();
  while (millis() - start < 1000) {
    while (mySerial.available()) {
      if (_gps.encode(mySerial.read())) {
        newData = true;
      }
    }
  }
  return newData;
}

void selectChannel(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          
  Wire.endTransmission();
  // Serial.println(bus);
}

float filteredCurrent(float dcCurrent, int filterNum) {
  float filteredCurrent = _kalmanFilter.updateEstimate(dcCurrent);
  for (int i = 0; i < filterNum; i++)
  {
    filteredCurrent = _kalmanFilter.updateEstimate(filteredCurrent);
  }

  return filteredCurrent;
}