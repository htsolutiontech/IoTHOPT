#include "SHT30.h"

#define DEFAULT_ADDRESS 0X44
#define COMMAND_MEASUREMENT_HIGH_REPEATABILITY 0x24

SHT30::SHT30() : _address(DEFAULT_ADDRESS), _temperature(NAN), _humidity(NAN) {}

SHT30::SHT30(uint8_t address) : _address(address), _temperature(NAN), _humidity(NAN) {}

void SHT30::begin(uint8_t address, TwoWire *wire)
{
    _address = address;
    _wire = wire;
    _wire->begin();
}

float SHT30::readTemperature()
{
    if (readData())
    {
        return _temperature;
    }
    return NAN;
}

float SHT30::readHumidity()
{
    if (readData())
    {
        return _humidity;
    }
    return NAN;
}

bool SHT30::readData()
{
    _wire->beginTransmission(_address);
    _wire->write(COMMAND_MEASUREMENT_HIGH_REPEATABILITY); // Send MSB
    _wire->write(0x00); // Send LSB
    _wire->endTransmission();
    delay(100);

    _wire->requestFrom(_address, (uint8_t)6);

    if (_wire->available() == 6)
    {
        uint16_t rawTemp = (_wire->read() << 8 | _wire->read());
        _wire->read(); // Discard CRC
        uint16_t rawHumi = (_wire->read() << 8 | _wire->read());
        _wire->read(); // Discard CRC

        _temperature = -45 + 175 * ((float)rawTemp / 65535.0);
        _humidity = 100 * ((float)rawHumi / 65535.0);

        return true;
    }

    return false;
}