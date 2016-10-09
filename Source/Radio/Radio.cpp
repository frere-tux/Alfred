#include "Radio.h"

#include <sys/time.h>
#include <time.h>

#include "Wiring/Wiring.h"

using namespace Al;

unsigned int Radio::getPulseIn(int _pin, int _level, int _timeout /*=500000*/)
{
    struct timeval tn, t0, t1;
    long micros;
    gettimeofday(&t0, NULL);
    micros = 0;

    while (Wiring::readDigital(_pin) != _level)
    {
        gettimeofday(&tn, NULL);
        if (tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0;
        }

        micros += (tn.tv_usec - t0.tv_usec);
        if (micros > _timeout)
        {
            return 0;
        }
    }

    gettimeofday(&t1, NULL);
    while (Wiring::readDigital(_pin) == _level)
    {
        gettimeofday(&tn, NULL);
        if (tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0;
        }

        micros = micros + (tn.tv_usec - t0.tv_usec);
        if (micros > _timeout)
        {
            return 0;
        }
    }

    if (tn.tv_sec > t1.tv_sec)
    {
        micros = 1000000L;
    }
    else
    {
        micros = 0;
    }

    micros = micros + (tn.tv_usec - t1.tv_usec);
    return micros;
}
