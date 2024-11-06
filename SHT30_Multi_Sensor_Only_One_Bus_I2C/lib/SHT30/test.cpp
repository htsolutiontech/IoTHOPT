#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <SHT30.h>
#include <TinyGPS++.h>
#include <WCS1800.h>

SHT30 sensor;

#define I2C_SDA 21
#define I2C_SCL 22

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          
  Wire.endTransmission();
  Serial.println(bus);
}

void setup() {
  Serial.begin(9600); 
  
  Wire.begin(I2C_SDA, I2C_SCL);
  
  sensor.begin(0x44, &Wire);
}

void loop() {

  TCA9548A(4);
  float temperature = sensor.readTemperature();
  
  float humidity = sensor.readHumidity();
  
  if (!isnan(temperature) && !isnan(humidity)) {
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
   
    Serial.println("Failed to read from SHT30 sensor!");
  }

  delay(2000); 
}
