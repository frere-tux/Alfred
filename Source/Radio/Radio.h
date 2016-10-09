#pragma once

namespace Al
{
    class Radio
    {
    public:
        Radio() {}
        ~Radio() {}

        // Get input pulse in microseconds
        static unsigned int getPulseIn(int _pin, int _level, int _timeout = 500000);
    };
}
