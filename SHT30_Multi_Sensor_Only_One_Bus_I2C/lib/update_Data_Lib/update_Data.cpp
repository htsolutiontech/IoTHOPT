#include <update_Data.h>

String URL = "https://iomt.hoangphucthanh.vn/";

void upload_Data(float T1, float H1, float T2, float H2, float T3, float H3, float current, float nowPower, double lat, double lng,
                 uint32_t LightToggleCount, boolean opr_flag, String _OPR_Time, String _Time)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(URL);
    // http.begin("iomt.hoangphucthanh.vn", 443, "/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature1=" + String(T1) + "&humidityAir1=" + String(H1) + "&temperature2=" + String(T2) 
                      + "&humidityAir2=" + String(H2) + "&temperature3=" + String(T3) + "&humidityAir3=" + String(H3) 
                      + "&lon=" + String(lng, 6) + "&lat=" + String(lat, 6) + "&opr_flag=" + String(opr_flag) 
                      + "&currentValue=" + String(current) + "&powerValue=" + String(nowPower) + "&lightToggleCount=" 
                      + String(LightToggleCount) + "&oprtime=" + _OPR_Time + "&time=" + _Time;

    int httpResponseCode = http.POST(postData);
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