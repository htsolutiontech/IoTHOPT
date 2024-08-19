#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// WiFi credentials
#define ssid "HOPT_TẦNG 2"
#define password "H@pt2020"

// API URL (Đường dẫn đến PHP script trên server)
#define API_URL "https://iomt.hoangphucthanh.vn/"

// Timing variables
const long interval = 5000;  // Thời gian gửi dữ liệu (5 giây)
unsigned long previousMillis = 0;

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

void setup_ntp() {
  configTime(0, 0, "pool.ntp.org");  // Set NTP server
  Serial.println("Waiting for time synchronization");
  while (!time(nullptr)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nTime synced successfully");
}

void sendData(float T, float H, String currentTime) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(API_URL); // URL của PHP script trên server

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Chuẩn bị dữ liệu để gửi
    String postData = "temperature=" + String(T) + "&humidityAir=" + String(H) + "&time=" + currentTime;

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from server: OKE ");
    } else {
      Serial.println("Error in sending data, HTTP code: " + String(httpResponseCode));
    }

    http.end(); // Giải phóng tài nguyên
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Sinh dữ liệu ngẫu nhiên cho nhiệt độ và độ ẩm
    float temperature = random(200, 300) / 10.0; // Nhiệt độ ngẫu nhiên từ 20.0 đến 30.0°C
    float humidity = random(400, 800) / 10.0;    // Độ ẩm ngẫu nhiên từ 40.0 đến 80.0%

    // Lấy thời gian hiện tại
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    String currentTime = String(timeString);

    // In dữ liệu ra Serial Monitor
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Time: ");
    Serial.println(currentTime);

    // Gửi dữ liệu tới server
    sendData(temperature, humidity, currentTime);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_ntp();  // Đồng bộ với server NTP để lấy thời gian chính xác
}
