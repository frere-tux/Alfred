#include "Wiring.h"

#include    "Managers/DebugManager.h"

using namespace Al;

bool Wiring::init()
{
    DebugManager debug;
    if(wiringPiSetup() == -1)
    {
        debug.addLog(LogType_Error, "Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return false;
    }

    debug.addLog(LogType_Message,"WiringPI lib initialized");
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
