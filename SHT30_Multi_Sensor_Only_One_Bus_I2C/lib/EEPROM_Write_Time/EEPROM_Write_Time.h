
#ifndef ___EEPROM_WRITE_TIME_H___
#define ___EEPROM_WRITE_TIME_H___

#include <EEPROM.h>

extern uint64_t t_Store_OPR_Time;
extern volatile boolean last_time_Flag;

extern volatile boolean time_Flag;

extern hw_timer_t *timer;

extern String OPR_Time;

extern uint32_t on_Off_Light_Count;

extern const uint8_t COUNT_ON_OFF_LIGHT_ADDRESS;

extern void displayActiveTime();

extern void EEPROM_Init();

extern void saveTime();

extern void EEPROMTimeHandler_Init();

extern void IRAM_ATTR count_OPR_Time();

extern void update_Light_State();

#endif // ___EEPROM_WRITE_TIME_H___
