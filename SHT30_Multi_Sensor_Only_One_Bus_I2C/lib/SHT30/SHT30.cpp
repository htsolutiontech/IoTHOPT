#include "SHT30.h"
#include <Wire.h>

static bool SHT30_readData(SHT30 *sensor) {
    Wire.beginTransmission(sensor->address);
    Wire.write(COMMAND_MEASUREMENT_HIGH_REPEATABILITY); // Send MSB
    Wire.write(0x00); // Send LSB
    Wire.endTransmission();
    delay(100);

    Wire.requestFrom(sensor->address, (uint8_t)6);

    if (Wire.available() == 6) {
        uint16_t rawTemp = (Wire.read() << 8 | Wire.read());
        Wire.read(); // Discard CRC
        uint16_t rawHumi = (Wire.read() << 8 | Wire.read());
        Wire.read(); // Discard CRC

        sensor->temperature = -45 + 175 * ((float)rawTemp / 65535.0);
        sensor->humidity = 100 * ((float)rawHumi / 65535.0);

        return true;
    }

    return false;
} 


void SHT30_init(SHT30 *sensor, uint8_t address) {
    sensor->address = address;
    sensor->temperature = NAN;
    sensor->humidity = NAN;
}

void SHT30_begin(SHT30 *sensor) {
    Wire.begin();
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
