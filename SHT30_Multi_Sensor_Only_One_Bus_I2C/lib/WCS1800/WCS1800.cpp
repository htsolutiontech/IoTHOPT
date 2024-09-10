#include "WCS1800.h"

const float WCS1800::SENSOR_VOLTAGE_REF = 3.32;
const float WCS1800::SENSOR_OFFSET_VOLTAGE = 1.68;
const float WCS1800::SENSITIVITY = 0.062;
const int WCS1800::ADC_RESOLUTION = 4096;

WCS1800::WCS1800(int pin) : _pin(pin), _midPoint(0) {}

void WCS1800::begin() {
    pinMode(_pin, INPUT);
}

void WCS1800::readData() {
    readADCBuffer(_data);
    int32_t dataSum = 0;
    for (int i = 0; i < 120; i++)
    {
        dataSum += _data[i];
    }
    _midPoint = dataSum / 120;
    
}

void WCS1800::readADCBuffer(int16_t* rawData) {
    analogRead(_pin);
    delay(10);

    for (int i = 0; i < 120; i++)
    {
        rawData[i] = analogRead(_pin);
        delayMicroseconds(829);
    }
    
}

float WCS1800::getCurrent() {
    // Serial.println(_midPoint);
    float voltage = (_midPoint / (float)ADC_RESOLUTION) * SENSOR_VOLTAGE_REF;
    // Serial.println(voltage);
    float current = (voltage - SENSOR_OFFSET_VOLTAGE) / SENSITIVITY;
    
    return current;
}

