/*
    * Creator: Ryan.
    * Jan, 6, 2025.
*/
#include <Arduino.h>

#ifndef ___A7680C_SIM_Lib___
#define ___A7680C_SIM_Lib___

/* Module Sim */
#define RX_SIM_PIN 16
#define TX_SIM_PIN 17

#define NUMBER_PHONE "0769714094"

#define  POWER_WARNING "WARNING: MACHINE OVERLOADED! STOP AND CHECK IMMEDIATELY FOR SAFETY."

#define TEMP_WARNING "WARNING: MACHINE OVERHEATED! STOP AND CHECK IMMEDIATELY FOR SAFETY."
extern HardwareSerial SIM_Serial;

extern void sendATCommand(const char* cmd, const char* expectedResponse, unsigned long timeout);

extern void sendSMS(String number, String message);

extern void SIM_Setup();

#endif // ___A7680C_SIM_Lib___