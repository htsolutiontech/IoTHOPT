
#ifndef ___EEPROM_WRITE_TIME_H___
#define ___EEPROM_WRITE_TIME_H___

#include <EEPROM.h>

#define ADDRESS_STORE_OPR_TIME 0
#define ADDRESS_STORE_MACHINE_SWITCH_COUNT 8

extern uint64_t _updateLampStateInterval;

extern volatile boolean _updateLampStatFlag;

extern uint64_t total_Seconds_OPR_Time;

extern volatile boolean last_time_Flag;

extern volatile boolean time_Flag;

extern hw_timer_t *timer;

extern hw_timer_t *buzzerTimer;

extern String OPR_Time;

extern uint32_t total_Switch_Machine_Count;

extern const uint8_t COUNT_ON_OFF_LIGHT_ADDRESS;

extern void displayActiveTime();

extern void EEPROM_Setup();

extern void saveTime();

extern void EEPROM_Time_Handler_Setup();

extern void IRAM_ATTR count_OPR_Time();

extern void update_Lamp_State();

extern void reset_Address_Eeprom();

#endif // ___EEPROM_WRITE_TIME_H___
