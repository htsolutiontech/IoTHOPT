#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <Wire.h>

// WiFi credentials
#define ssid "HOPT_TẦNG 2"
#define password "H@pt2020"
#define SDA_PIN 21
#define SCL_PIN 22

// API URL (Đường dẫn đến PHP script trên server)
#define API_URL "https://iomt.hoangphucthanh.vn/"

// Timing variables
const long interval = 5000;  // Thời gian gửi dữ liệu (1 giây)
unsigned long previousMillis = 0;

// Thiết lập kết nối WiFi
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

// Đọc dữ liệu từ cảm biến SHT3x
bool readSHT3x(float &temperature, float &humidity) {
  Wire.beginTransmission(0x44); // Địa chỉ I2C cho SHT3x
  Wire.write(0x24);  // Lệnh đo nhiệt độ và độ ẩm
  Wire.write(0x00);
  Wire.endTransmission();
  delay(500); // Đợi cảm biến đo

  Wire.requestFrom(0x44, 6);
  if (Wire.available() == 6) {
    uint16_t tData = Wire.read() << 8 | Wire.read();
    Wire.read(); // Byte CRC, bỏ qua
    uint16_t hData = Wire.read() << 8 | Wire.read();
    Wire.read(); // Byte CRC, bỏ qua

    // Chuyển đổi dữ liệu thô thành nhiệt độ và độ ẩm
    temperature = -45 + 175 * ((float)tData / 65535);
    humidity = 100 * ((float)hData / 65535);

    return true;
  }
  return false;
}

// Thiết lập NTP và cấu hình múi giờ Việt Nam (GMT+7)
void setup_ntp() {
  configTime(7 * 3600, 0, "pool.ntp.org");  // 7 * 3600 giây tương ứng với GMT+7
  Serial.println("Waiting for time synchronization");
  while (!time(nullptr)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nTime synced successfully");
}

// Gửi dữ liệu tới server
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
      Serial.println("Response from server:OKE ");
    } else {
      Serial.println("Error in sending data, HTTP code: " + String(httpResponseCode));
    }

    http.end(); // Giải phóng tài nguyên
  } else {
    Serial.println("Error in WiFi connection");
  }
}

// Vòng lặp chính
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Đọc dữ liệu từ cảm biến SHT3x
    float temperature = 0.0;
    float humidity = 0.0;
    if (readSHT3x(temperature, humidity)) {
      // Lấy thời gian hiện tại
      time_t now;
      struct tm timeinfo;
      time(&now);
      localtime_r(&now, &timeinfo);

      // Định dạng thời gian theo múi giờ Việt Nam
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
    } else {
      Serial.println("Failed to read from SHT3x sensor.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // Khởi tạo I2C với chân SDA và SCL
  setup_wifi();
  setup_ntp();  // Đồng bộ với server NTP để lấy thời gian chính xác
}
