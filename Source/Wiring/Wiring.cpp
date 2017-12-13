#include "Wiring.h"

#include <Debug/DebugManager.h>

namespace Al
{

bool Wiring::init()
{
    if(wiringPiSetup() == -1)
    {
        g_DebugManager->addLog(LogType_Error, "Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return false;
    }

    g_DebugManager->addLog(LogType_Message,"WiringPI lib initialized");
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

}
