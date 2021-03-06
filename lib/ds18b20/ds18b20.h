#ifndef __LIB_DS18B20_H
#define __LIB_DS18B20_H

#include <Wire.h>
#include <OneWire.h>
#include <freertos/task.h>

class DS18B20
{
public:
    using Resolution_t = enum {
        Resolution9b = 0,
        Resolution10b,
        Resolution11b,
        Resolution12b,
    };

    using Result_t = enum {
        OK,
        ERROR,
    };

private:
    uint8_t pin;
    OneWire oneWire;

    bool isInitialized = false;
    Resolution_t resolution = Resolution12b;
    byte address[8];

    const byte familyCode = 0x28;

public:
    DS18B20(uint8_t _pin) : pin(_pin), oneWire(_pin)
    {
    }

    void init();

    /**
     * Request to measure temperature
     * @return Converting time [ms]
     */
    uint32_t requestTemperature();

    /**
     * Get temperature in celcius
     * @return Temperature [degrees-C]
     */
    Result_t getTemperatureC(float *temp);
};

#endif