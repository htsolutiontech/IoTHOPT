#include "A7680C_SIM_Lib.h"

/* Module Sim */
HardwareSerial SIM_Serial(1);

void sendATCommand(const char* cmd, const char* expectedResponse, unsigned long timeout) {
  SIM_Serial.println(cmd);  // Gửi lệnh AT tới module SIM
  String response = "";
  unsigned long startTime = millis();  // Lưu thời gian bắt đầu
  bool responseOK = false;

  // Đọc phản hồi từ module SIM 
  while (millis() - startTime < timeout) {
    while (SIM_Serial.available() > 0) {
      char c = SIM_Serial.read();  
      response += c;  
    }
    if (response.indexOf(expectedResponse) != -1) { 
      responseOK = true;
      break; 
    }
  }
  Serial.println(response);  //phản hồi từ module SIM

  if (responseOK)
    Serial.println("Response OK");
  else
    Serial.println("Timeout without expected Response");
}


// Hàm gửi SMS
void sendSMS(String number, String message) {
  String cmd = "AT+CMGS=\"" + number + "\"\r\n";  
  SIM_Serial.print(cmd);  //số điện thoại
  delay(100);
  SIM_Serial.println(message);  //tin nhắn
  delay(100);
  SIM_Serial.write(0x1A); // Ctrl+Z 
  delay(100);
}


void SIM_Setup()
{
  SIM_Serial.begin(115200, SERIAL_8N1, RX_SIM_PIN, TX_SIM_PIN, true);

  sendATCommand("AT+CGMM", "OK", 1000); // Kiểm tra kết nối với module SIM
  sendATCommand("AT+CMGF=1", "OK", 1000); // Đặt chế độ SMS là văn bản

//   sendSMS(NUMBER_PHONE, POWER_WARNING);
}