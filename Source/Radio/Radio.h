#pragma once

#include "Tools/Plot.h"

#define RADIO_MESSAGE_SIZE 32

namespace Al
{
    typedef struct RadioManchesterArray {bool data[RADIO_MESSAGE_SIZE*2];} RadioManchesterArray;
    typedef struct RadioMessageArray {bool data[RADIO_MESSAGE_SIZE];} RadioMessageArray;

    class Radio
    {
    public:
        Radio(const unsigned int _receptorPin, const unsigned int _transmitterPin);
        ~Radio() {}

        void process();

        bool manchesterCheck(const RadioManchesterArray& _manchArray, const RadioManchesterArray& _validManchArray, RadioMessageArray& _msgArray, RadioMessageArray& _validMsgArray);
        bool tryGetMessage(RadioManchesterArray& _manchArray, RadioManchesterArray& _validArray, const bool _wait, const bool _preparePlot = false, const int _timeOut = 500000);
        void plotLastMessage();

        void sendBit(const bool _bit);
        void sendPair(const bool _bit);
        void transmit(const unsigned int _nbMsg, const bool _intOn, const bool _group, const unsigned int _intId);
        void transmit(const bool _intOn, const bool _group, const unsigned int _intId);

    private:
        // Get input pulse in microseconds
        unsigned int getPulseIn(int _timeout = 1000000);

        unsigned int    m_receptorPin;
        unsigned int    m_transmitterPin;

        Plot            m_plot;
    };
}
