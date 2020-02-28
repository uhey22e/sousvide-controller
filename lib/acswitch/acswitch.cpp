#include "acswitch.h"

void ACSwitch::turnOn()
{
    digitalWrite(swPin, HIGH);
}

void ACSwitch::turnOff()
{
    digitalWrite(swPin, LOW);
}