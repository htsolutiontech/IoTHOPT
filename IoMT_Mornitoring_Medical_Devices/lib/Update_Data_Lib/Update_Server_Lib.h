#ifndef ___update_Data___
#define ___update_Data___

#include <HTTPClient.h>

extern String URL;

extern int httpResponseCode;

extern void upload_Data(float T, float H, float K, float M, float current, float nowPower, double lat, double lng, boolean opr_flag, 
                  uint32_t LightToggleCount, String _OPR_Time,String curent_Time);


#endif // ___update_Data___

