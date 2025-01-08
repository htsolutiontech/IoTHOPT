#include <Update_Server_Lib.h>

String URL = "https://iomt.hoangphucthanh.vn/";

int httpResponseCode = 0;

void upload_Data(float T, float H, float K, float M, float current, float nowPower, double lat, double lng, boolean opr_flag, 
                  uint32_t LightToggleCount, String _OPR_Time, String curent_Time)
{

  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(URL);
    // http.begin("iomt.hoangphucthanh.vn", 443, "/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature=" + String(T) + "&humidityAir=" + String(H) 
                      + "&PPM_Value=" + String(K) + "&dustDensity=" + String(M)  
                      + "&lon=" + String(lng, 6) + "&lat=" + String(lat, 6) + "&opr_flag=" + String(opr_flag) 
                      + "&currentValue=" + String(current) + "&powerValue=" + String(nowPower) + "&lightToggleCount=" 
                      + String(LightToggleCount) + "&oprtime=" + _OPR_Time + "&time=" + curent_Time;

    httpResponseCode = http.POST(postData);
    // Serial.println("Data being sent: " + postData);


    if (httpResponseCode > 0)
    {
      String response = http.getString();
      // Serial.println(response);
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