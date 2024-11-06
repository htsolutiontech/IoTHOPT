#include <Wifi_Setup.h>

const char *ssid = "HOPT_TẦNG 2";
const char *password = "H@pt2020";

void Wifi_Init() {
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
  Serial.println("Signal strength (RSSI): " + String(WiFi.RSSI()) + " dBm");
   
}