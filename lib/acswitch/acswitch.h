#ifndef __LIB_ACSWITCH_H
#define __LIB_ACSWITCH_H

#include <Arduino.h>

class ACSwitch
{
private:
    uint8_t swPin;

public:
    ACSwitch(uint8_t _swPin) : swPin(_swPin)
    {
        pinMode(swPin, OUTPUT);
    }

    void turnOn();
    void turnOff();
};

#endif