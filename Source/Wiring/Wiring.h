#pragma once

#include <wiringPi.h>

namespace Al
{
    enum PinMode
    {
        PinMode_Input = INPUT,
        PinMode_Output = OUTPUT,
        PinMode_PWMOutput = PWM_OUTPUT,
        PinMode_GPIOClock = GPIO_CLOCK,
        PinMode_SoftPWMOutput = SOFT_PWM_OUTPUT,
        PinMode_SoftToneOutput = SOFT_TONE_OUTPUT,
        PinMode_PWMToneOutput = PWM_TONE_OUTPUT
    };

    class Wiring
    {
    public:
        Wiring() {}
        ~Wiring() {}

        static bool init();
        static void setPinMode(int _pin, int _mode);
        static int readDigital(int _pin);
        static void writeDigital(int _pin, int _value);
    };
}
