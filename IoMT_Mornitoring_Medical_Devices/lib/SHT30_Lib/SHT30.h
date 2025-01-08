#ifndef SHT30_H
#define SHT30_H


#include <stdint.h>
#include <stdbool.h>
#include <Wire.h>

#define DEFAULT_ADDRESS 0x44
#define COMMAND_MEASUREMENT_HIGH_REPEATABILITY 0x24

extern TwoWire SHT30_I2C;

typedef struct 
{
    uint8_t address;
    void (*begin)(uint8_t address);
    float temperature;
    float humidity;
} SHT30;

// Function prototypes
extern void SHT30_init(SHT30 *sensor, uint8_t address);
extern void SHT30_begin(SHT30 *sensor, uint8_t SHT30_I2C_SDA, uint8_t SHT30_I2C_SCL);
extern float SHT30_readTemperature(SHT30 *sensor);
extern float SHT30_readHumidity(SHT30 *sensor);

#endif