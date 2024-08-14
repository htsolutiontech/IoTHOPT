#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi credentials
#define ssid "HOPT_TẦNG 2"
#define password "H@pt2020"

// MQTT Broker details
#define MQTT_SERVER "171.251.89.96"
#define MQTT_PORT 8916
#define MQTT_TOPIC_PUB "smart_home_humidity_and_temperature"

// I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// MQTT and WiFi objects
WiFiClient espClient;
PubSubClient client(espClient);

// JSON document for MQTT message
StaticJsonDocument<200> mess_publish;

// Timing variables
const long interval = 5000;
unsigned long previousMillis = 0;

// Function to read SHT3x sensor
bool readSHT3x(float &temperature, float &humidity) {
  Wire.beginTransmission(0x44); // I2C address for SHT3x
  Wire.write(0x24);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(500); // Wait for measurement

  Wire.requestFrom(0x44, 6);
  if (Wire.available() == 6) {
    uint16_t tData = Wire.read() << 8 | Wire.read();
    Wire.read(); // CRC byte, ignore for now
    uint16_t hData = Wire.read() << 8 | Wire.read();
    Wire.read(); // CRC byte, ignore for now

    // Convert raw data to temperature and humidity
    temperature = -45 + 175 * ((float)tData / 65535);
    humidity = 100 * ((float)hData / 65535);

    return true;
  }
  return false;
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "472e4164-ab08-431a-9078-8fde55eb6e7c";
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C with custom SDA and SCL pins
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void Publish(float T, float H) {
  char buffer[200]; // Adjust buffer size as needed
  mess_publish["humidityAir"] = H;
  mess_publish["temperature"] = T;
  mess_publish["location"] = "Toa Trung Tam";
  serializeJson(mess_publish, buffer);
  bool check = client.publish(MQTT_TOPIC_PUB, buffer);
  if (check) {
    Serial.println("Publish successful");
  } else {
    Serial.println("Publish failed");
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temperature, humidity;
    if (readSHT3x(temperature, humidity)) {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.write("\xC2\xB0");
      Serial.println("C");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println("%");

      Publish(temperature, humidity);
    } else {
      Serial.println("Failed to read from sensor");
    }
  }
}
