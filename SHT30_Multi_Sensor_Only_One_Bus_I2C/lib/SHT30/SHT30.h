#ifndef SHT30_H
#define SHT30_H

#include <Wire.h>

class SHT30
{
public:
    SHT30();                // địa chỉ mặc định, không cần gán
    SHT30(uint8_t address); // gán địa chỉ khác nếu có
    void begin(uint8_t address, TwoWire *wire = &Wire);
    float readTemperature();
    float readHumidity();

private:
    uint8_t _address;
    TwoWire *_wire;
    float _temperature;
    float _humidity;

    bool readData();
};

#endif