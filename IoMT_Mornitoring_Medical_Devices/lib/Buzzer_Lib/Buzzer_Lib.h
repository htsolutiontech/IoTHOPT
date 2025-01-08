/*
    * Creator: Ryan.
    * December, 26, 2024.
*/
#include <Arduino.h>

#ifndef ___Buzzer_Lib___
#define ___Buzzer_Lib___

#define BUZZER_PIN 13  
#define PWM_CHANNEL 0  
#define PWM_FREQUENCY 2000  
#define PWM_RESOLUTION 8    
#define BUZZER_ON_TIME 300000  
#define BUZZER_OFF_TIME 100000

extern uint32_t lastBuzzerStartTime;
extern uint8_t lastBuzzerStartInterval;

extern uint32_t lastBuzzerWarningTime;
extern uint16_t lastBuzzerWarningInterval;

// extern volatile bool buzzerState;
 
extern void buzzer_Setup();

extern void buzzer_Start();

extern void buzzer_Warning();

extern void buzzer_ON();

extern void buzzer_OFF();


#endif // ___Buzzer_Lib___