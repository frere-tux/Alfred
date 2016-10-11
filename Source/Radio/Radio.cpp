#include "Radio.h"

#include <sys/time.h>
#include <time.h>

#include "Wiring/Wiring.h"

using namespace Al;

Radio::Radio(const unsigned int _receptorPin, const unsigned int _transmitterPin)
    : m_receptorPin(_receptorPin)
    , m_transmitterPin(_transmitterPin)
{

}

unsigned int Radio::getPulseIn(int _timeout /*=500000*/)
{
    struct timeval tn, t0, t1;
    long micros;
    gettimeofday(&t0, NULL);
    micros = 0;

    while (Wiring::readDigital(m_receptorPin) != LOW)
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
    while (Wiring::readDigital(m_receptorPin) == LOW)
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

bool Radio::tryGetMessage(RadioManchesterArray& _manchArray, RadioManchesterArray& _validArray, const bool _wait, const bool _preparePlot /*= false*/, const int _timeOut /*= 500000*/)
{
    unsigned long t = 0;
    unsigned long tCorrection = 0;

    if (_preparePlot)
    {
        m_plot.init();
    }

    // Wait for latch
    t = Radio::getPulseIn(_timeOut);
    while(t < 2550 || t > 2800)
    {
        if (!_wait)
        {
            return false;
        }

        t = Radio::getPulseIn(_timeOut);
    }

    if (_preparePlot)
    {
        m_plot.logPulse(t);
    }

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        t = Radio::getPulseIn(_timeOut) + tCorrection;

        if (_preparePlot)
        {
            m_plot.logPulse(t);
        }

        if(t > 200 && t < 1000)
        {
            _manchArray.data[dataBits] = 0;
            _validArray.data[dataBits] = 1;

            tCorrection = 0;
        }
        else if(t > 1000 && t < 2000)
        {
            _manchArray.data[dataBits] = 1;
            _validArray.data[dataBits] = 1;

            tCorrection = 0;
        }
        else
        {
            if (t <= 200)
            {
                tCorrection = t;
                --dataBits;
            }
            else
            {
                _validArray.data[dataBits] = 0;
            }
        }
    }

    if (_preparePlot)
    {
        m_plot.stopLog();
    }

    return true;
}

bool Radio::manchesterCheck(const RadioManchesterArray& _manchArray, const RadioManchesterArray& _validManchArray, RadioMessageArray& _msgArray, RadioMessageArray& _validMsgArray)
{
    bool returnValue = true;

    bool prevBit = 0;
    bool prevValid = 1;

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        bool bit = _manchArray.data[dataBits];
        bool valid = _validManchArray.data[dataBits];

        if (valid == 0)
        {
            prevBit = bit;
            prevValid = valid;
            continue;
        }

        if(dataBits % 2 == 1 && prevValid == 1)
        {
            if((prevBit ^ bit) == 0)
            {
                _validMsgArray.data[(dataBits -1)/2] = 0;
                returnValue = false;
            }
            else
            {
                _validMsgArray.data[(dataBits -1)/2] = 1;
            }
            _msgArray.data[(dataBits -1)/2] = prevBit;
        }

        prevBit = bit;
        prevValid = valid;
    }

    return returnValue;
}

void Radio::plotLastMessage()
{
    m_plot.plotLog();
}
