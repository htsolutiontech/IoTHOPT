#ifndef WCS1800_h
#define WCS1800_h

#include <Arduino.h>

class WCS1800 {
public:
    WCS1800(int pin);

    void begin();

    void readData();

    float getCurrent();

private:
    int _pin;

    int16_t _data[120];

    int16_t _midPoint;

    void readADCBuffer(int16_t* rawData);

    static const float SENSOR_VOLTAGE_REF;
    static const float SENSOR_OFFSET_VOLTAGE;
    static const float SENSITIVITY;
    static const int ADC_RESOLUTION;

};

#endif
