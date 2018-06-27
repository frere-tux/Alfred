#pragma once

#include <thread>
#include <vector>
#include <map>
#include <mutex>

#include <Debug/Plot.h>
#include <Managers/Manager.h>

#include "Transmission.h"
#include "RadioTypes.h"

#define RADIO_MESSAGE_SIZE 32

namespace Al
{
    typedef struct RadioManchesterArray {bool data[RADIO_MESSAGE_SIZE*2];} RadioManchesterArray;
    typedef struct RadioMessageArray {bool valid = true; bool data[RADIO_MESSAGE_SIZE];} RadioMessageArray;

    class RadioManager : public Manager
    {
    public:
        RadioManager();
        ~RadioManager() {}

        virtual void   Init() override;

        virtual void   Process(float _dt) override;
        void            AsyncProcessReception();
        void            AsyncProcessTransmission();

        virtual void   End();

        TransmissionId   AddTransmission(const TransmissionParam& _param);
        void            RemoveTransmission(TransmissionId _id);

    private:

        bool            manchesterCheck(const RadioManchesterArray& _manchArray, const RadioManchesterArray& _validManchArray, RadioMessageArray& _msgArray, RadioMessageArray& _validMsgArray);
        bool            tryGetMessage(RadioManchesterArray& _manchArray, RadioManchesterArray& _validArray, const bool _wait, const bool _preparePlot = false, const int _timeOut = 500000);
        void            plotLastMessage();

        void            sendBit(const bool _bit);
        void            sendPair(const bool _bit);
        void            Transmit(const unsigned int _nbMsg, const bool _intOn, const bool _group, const unsigned int _roomId, const unsigned int _objectId);
        void            Transmit(const bool _intOn, const bool _group, const unsigned int _groupId, const unsigned int _intId);


        // Get input pulse in microseconds
        unsigned int   getPulseIn(int _timeout = 500000);

        unsigned int    m_receptorPin;
        unsigned int    m_transmitterPin;

        Plot            m_plot;

        std::thread*    m_receptionThread;
        std::mutex      m_receptionMutex;
        std::vector<RadioMessageArray>  m_messagesReceived;

        std::map<TransmissionId, Transmission> m_transmissions;

        std::mutex m_transmissionsMutex;
    };
}
