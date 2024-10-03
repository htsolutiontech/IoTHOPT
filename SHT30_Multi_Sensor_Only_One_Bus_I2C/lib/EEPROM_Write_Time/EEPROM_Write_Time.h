
#ifndef ___EEPROM_WRITE_TIME_H___
#define ___EEPROM_WRITE_TIME_H___

#include <EEPROM.h>

extern uint32_t t_Store_OPR_Time;
extern bool isSaved;
extern bool isStarted;

extern volatile boolean time_Flag;

extern hw_timer_t *timer;

extern String OPR_Time;

extern uint32_t on_Off_Light_Count;

extern const uint8_t COUNT_ON_OFF_LIGHT_ADDRESS;

extern void displayActiveTime();

extern void EEPROM_Setup();

extern void saveTime();

extern void Handler_EEP_Setup();

extern void IRAM_ATTR count_OPR_Time();

extern void update_Light_State();

#endif // ___EEPROM_WRITE_TIME_H___
