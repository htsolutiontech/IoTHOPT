/*
    * Creator: Ryan.
    * September, 10, 2024.
*/

#ifndef HSTS016L_Lib_H
#define HSTS016L_Lib_H

#include <Arduino.h>

#define DEFAULT_R 10000
#define DEFAULT_Q 0.1
#define ADC_RESOLUTION 4095
#define SUPPLY_VOLTAGE 3300
#define MV_PER_AMP 12.5

extern float currentFiltered;
extern float calibFiltered;

extern uint32_t previousTime;
extern uint32_t sampleIntervalMicros; 

typedef struct
{
    float Xt_current_prev;
    float Pt_current_prev;
    float Xt_calib_prev;
    float Pt_calib_prev; 
} HSTS016L_KalmanState_t;

typedef struct {
    uint8_t currentPin;
    uint8_t calibPin;
    HSTS016L_KalmanState_t kalmanState;
    float rValue;  // Process noise covariance
    float qValue;  // Measurement noise covariance
} HSTS016L_Config_t;

extern void HSTS016L_Init(HSTS016L_Config_t *config, uint8_t currentPin, uint8_t calibPin, float rValue, float qValue);
extern void HSTS016L_InitKalmanState(HSTS016L_Config_t *config, uint16_t initCurrent, uint16_t initCalib);

extern float HSTS016L_KalmanUpdateCurrent(HSTS016L_Config_t *config, uint16_t inputData);
extern float HSTS016L_KalmanUpdateCalib(HSTS016L_Config_t *config, uint16_t inputData);

extern float HSTS016L_ReadDCCurrent(HSTS016L_Config_t *config, uint16_t numSamples, uint16_t sampleInterval, float currentOffset);

#endif