#include "ds18b20.h"

void DS18B20::init()
{
    oneWire.reset_search();
    oneWire.reset();

    int searchAttempts;
    for (searchAttempts = 0; searchAttempts < 3; searchAttempts++)
    {
        if (!oneWire.search(address))
        {
            log_w("Device is not found.");
            continue;
        }

        if (OneWire::crc8(address, 7) != address[7])
        {
            log_w("CRC is not valid.");
            continue;
        }

        if (address[0] != familyCode)
        {
            log_w("It's not a DS18B20.");
            continue;
        }

        break;
    }

    if (searchAttempts >= 3)
    {
        log_e("Failed to find DS18B20.");
        abort();
    }

    isInitialized = true;
}

uint32_t DS18B20::requestTemperature()
{
    oneWire.reset();
    oneWire.select(address);
    // start conversion
    oneWire.write(0x44, 0);

    uint32_t conversionTime;
    switch (resolution)
    {
    case Resolution9b:
        conversionTime = 94;
        break;
    case Resolution10b:
        conversionTime = 188;
        break;
    case Resolution11b:
        conversionTime = 375;
        break;
    case Resolution12b:
        conversionTime = 750;
        break;
    default:
        conversionTime = 0;
        break;
    }
    return conversionTime;
}

DS18B20::Result_t DS18B20::getTemperatureC(float *temp)
{
    oneWire.reset();
    oneWire.select(address);
    // Read Scratchpad
    oneWire.write(0xBE);

    byte data[9];
    for (int i = 0; i < 9; i++)
    {
        // we need 9 bytes
        data[i] = oneWire.read();
    }

    // Check CRC
    // if (OneWire::crc8(data, 8) != data[8])
    // {
    //     log_e("CRC is not valid.");
    //     return ERROR;
    // }

    // Conversion
    int16_t rawData = (data[1] << 8) | data[0];

    switch ((data[4] & 0x60) >> 5)
    {
    case Resolution9b:
        // drop least 3bit
        rawData &= ~0x0007;
        break;
    case Resolution10b:
        // drop least 2bit
        rawData &= ~0x0003;
        break;
    case Resolution11b:
        // drop least 1bit
        rawData &= ~0x0001;
        break;
    }

    *temp = (float)rawData / 16.0;
    return OK;
}
