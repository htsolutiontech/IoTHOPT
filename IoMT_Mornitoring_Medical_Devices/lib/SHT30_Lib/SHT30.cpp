#include "SHT30.h"
#include <Wire.h>

TwoWire SHT30_I2C = TwoWire(0);

static bool SHT30_readData(SHT30 *sensor) {
    SHT30_I2C.beginTransmission(sensor->address);
    SHT30_I2C.write(COMMAND_MEASUREMENT_HIGH_REPEATABILITY); // Send MSB
    SHT30_I2C.write(0x00); // Send LSB
    SHT30_I2C.endTransmission();
    delay(100);

    SHT30_I2C.requestFrom(sensor->address, (uint8_t)6);

    if (SHT30_I2C.available() == 6) {
        uint16_t rawTemp = (SHT30_I2C.read() << 8 | SHT30_I2C.read());
        SHT30_I2C.read(); // Discard CRC
        uint16_t rawHumi = (SHT30_I2C.read() << 8 | SHT30_I2C.read());
        SHT30_I2C.read(); // Discard CRC

        sensor->temperature = -45 + 175 * ((float)rawTemp / 65535.0);
        sensor->humidity = 100 * ((float)rawHumi / 65535.0);

        return true;
    }

    return false;
} 

void SHT30_begin(SHT30 *sensor, uint8_t SHT30_I2C_SDA, uint8_t SHT30_I2C_SCL) {
    SHT30_I2C.begin(SHT30_I2C_SDA, SHT30_I2C_SCL);
}

void SHT30_init(SHT30 *sensor, uint8_t address) {
    sensor->address = address;
    sensor->temperature = NAN;
    sensor->humidity = NAN;
}

float SHT30_readTemperature(SHT30 *sensor) {
    if (SHT30_readData(sensor)) {
        return sensor->temperature;
    }
    return NAN;
}

float SHT30_readHumidity(SHT30 *sensor) {
    if (SHT30_readData(sensor)) {
        return sensor->humidity;
    }
    return NAN;
}
