/*
    * Creator: Ryan.
    * September, 10, 2024.
*/

#include "HSTS016L_Lib.h"

float currentFiltered = 0.0;
float calibFiltered = 0.0;


void HSTS016L_Init(HSTS016L_Config_t *config, uint8_t currentPin, uint8_t calibPin, float rValue, float qValue) {
    config->currentPin = currentPin;
    config->calibPin = calibPin;
    config->rValue = rValue;
    config->qValue = qValue;
}

void HSTS016L_InitKalmanState(HSTS016L_Config_t *config, uint16_t initCurrent, uint16_t initCalib) {
    config->kalmanState.Pt_current_prev = initCurrent;
   
    config->kalmanState.Pt_calib_prev = initCalib;
}

// Cập nhật Kalman cho giá trị dòng điện
float HSTS016L_KalmanUpdateCurrent(HSTS016L_Config_t *config, uint16_t inputData) {
    float Xt_update = config->kalmanState.Xt_current_prev;
    float Pt_update = config->kalmanState.Pt_current_prev + config->qValue;

    float Kt = Pt_update / (Pt_update + config->rValue);
    float Xt = Xt_update + Kt * (inputData - Xt_update);
    float Pt = (1 - Kt) * Pt_update;

    config->kalmanState.Xt_current_prev = Xt;
    config->kalmanState.Pt_current_prev = Pt;

    return Xt;
}

// Cập nhật Kalman cho giá trị hiệu chuẩn
float HSTS016L_KalmanUpdateCalib(HSTS016L_Config_t *config, uint16_t inputData) {
    float Xt_update = config->kalmanState.Xt_calib_prev;
    float Pt_update = config->kalmanState.Pt_calib_prev + config->qValue;

    float Kt = Pt_update / (Pt_update + config->rValue);
    float Xt = Xt_update + Kt * (inputData - Xt_update);
    float Pt = (1 - Kt) * Pt_update;

    config->kalmanState.Xt_calib_prev = Xt;
    config->kalmanState.Pt_calib_prev = Pt;

    return Xt;
}


float HSTS016L_ReadDCCurrent(HSTS016L_Config_t *config, uint16_t numSamples, uint16_t sampleInterval, float currentOffset) {
    unsigned long currentTime;
    float currentSampleSum = 0;
    float currentSampleCount = 0;
    float currentSampleRead = 0;
    float finalDCCurrent = 0;
    uint16_t currentPinRead = 0;
    uint16_t calibPinRead = 0;

    for (int i = 0; i < numSamples; i++) {
        currentTime = micros();

        currentPinRead = analogRead(config->currentPin);
        calibPinRead = analogRead(config->calibPin);

        currentFiltered = HSTS016L_KalmanUpdateCurrent(config, currentPinRead);
        calibFiltered = HSTS016L_KalmanUpdateCalib(config, calibPinRead);
        

        currentSampleRead = abs(currentFiltered - calibFiltered);
        currentSampleSum += currentSampleRead;
        currentSampleCount++;

        delayMicroseconds(sampleInterval);
    }

    float currentMean = currentSampleSum / currentSampleCount;
    finalDCCurrent = (((currentMean / ADC_RESOLUTION) * SUPPLY_VOLTAGE) / MV_PER_AMP) - currentOffset;

    return finalDCCurrent;
}
