#include "Wiring.h"

#include    "Managers/DebugManager.h"

using namespace Al;

bool Wiring::init()
{
    if(wiringPiSetup() == -1)
    {
        Debug::getInstance().addLog(LogType_Error, "Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return false;
    }

    Debug::getInstance().addLog(LogType_Message,"WiringPI lib initialized");
    return true;
}

void Wiring::setPinMode(int _pin, int _mode)
{
    pinMode(_pin, _mode);
}

int Wiring::readDigital(int _pin)
{
    return digitalRead(_pin);
}

void Wiring::writeDigital(int _pin, int _value)
{
    digitalWrite(_pin, _value);
}
