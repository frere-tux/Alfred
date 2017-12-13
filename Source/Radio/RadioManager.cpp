#include "RadioManager.h"

#include <sys/time.h>
#include <time.h>
#include <sstream>
#include <cmath>
#include <iostream>

#include <Wiring/Wiring.h>
#include <Debug/DebugManager.h>
#include <Objects/ObjectsManager.h>
#include <Config/ConfigManager.h>

#define PLOT_DATA false
#define PLOT_DATA_ON_FAIL false

#define SIGNAL_ERROR_EPSILON 250

#define SIGNAL_INTER_TIME 275 //310 orinally,  but tweaked
#define SIGNAL_ZERO_TIME 400  //310 orinally, but tweaked
#define SIGNAL_ONE_TIME 1225  //1340 orinally, but tweaked

namespace Al
{

RadioManager::RadioManager()
    : m_receptorPin(2)
    , m_transmitterPin(7)
{

}

void RadioManager::Init()
{
    m_receptorPin = g_ConfigManager->GetReceptorPin();
    m_transmitterPin = g_ConfigManager->GetEmitterPin();
}

unsigned int RadioManager::getPulseIn(int _timeout /*=500000*/)
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

bool RadioManager::tryGetMessage(RadioManchesterArray& _manchArray, RadioManchesterArray& _validArray, const bool _wait, const bool _preparePlot /*= false*/, const int _timeOut /*= 500000*/)
{
    unsigned long t = 0;
    unsigned long tCorrection = 0;

    if (_preparePlot)
    {
        m_plot.init();
    }

    // Wait for latch 2
    t = RadioManager::getPulseIn(_timeOut);
    while(t < 2000 || t > 3500)
    {
        if (!_wait)
        {
            return false;
        }

        t = RadioManager::getPulseIn(_timeOut - t);
    }

    if (_preparePlot)
    {
        m_plot.logPulse(t);
    }

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        t = RadioManager::getPulseIn(_timeOut - t) + tCorrection;

        if (_preparePlot)
        {
            m_plot.logPulse(t);
        }

        if(t > SIGNAL_ZERO_TIME-SIGNAL_ERROR_EPSILON && t < SIGNAL_ZERO_TIME+SIGNAL_ERROR_EPSILON*2)
        {
            _manchArray.data[dataBits] = 0;
            _validArray.data[dataBits] = 1;

            tCorrection = 0;
        }
        else if(t > SIGNAL_ONE_TIME-SIGNAL_ERROR_EPSILON && t < SIGNAL_ONE_TIME+SIGNAL_ERROR_EPSILON*2)
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

    /*for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        //std::cout << _manchArray.data[dataBits];

        _validArray.data[dataBits] = 1;
    }
    //std::cout << std::endl;

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        //std::cout << _validArray.data[dataBits];
    }
    //std::cout << std::endl;*/

    if (_preparePlot)
    {
        m_plot.stopLog();
    }

    return true;
}

bool RadioManager::manchesterCheck(const RadioManchesterArray& _manchArray, const RadioManchesterArray& _validManchArray, RadioMessageArray& _msgArray, RadioMessageArray& _validMsgArray)
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

            /*if (_validMsgArray.data[(dataBits -1)/2] == 1)
            {
                std::cout << prevBit;
            }
            else
            {
                std::cout << "x";
            }*/
        }

        prevBit = bit;
        prevValid = valid;
    }
    //std::cout << std::endl;
    return returnValue;
}

void RadioManager::plotLastMessage()
{
    m_plot.plotLog();
}

void RadioManager::Process(float _dt)
{
    unsigned long sender = 0;
    bool group = false;
    bool on = false;
    unsigned int recipient = 0;

    RadioManchesterArray manchArray, validManchArray;
    for (unsigned int i = 0 ; i < RADIO_MESSAGE_SIZE*2 ; ++i)
    {
        manchArray.data[i] = 0;
        validManchArray.data[i] = 0;
    }

    RadioMessageArray msgArray, validMsgArray;
    for (unsigned int i = 0 ; i < RADIO_MESSAGE_SIZE ; ++i)
    {
        msgArray.data[i] = 0;
        validMsgArray.data[i] = 0;
    }

    if (tryGetMessage(manchArray, validManchArray, true, PLOT_DATA))
    {
        if(manchesterCheck(manchArray, validManchArray, msgArray, validMsgArray))
        {
            if (PLOT_DATA && !PLOT_DATA_ON_FAIL)
            {
                plotLastMessage();
            }

            for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE ; ++dataBits)
            {
                if(dataBits <= 25)
                {
                    // first 26 data bits
                    sender <<= 1;
                    sender |= msgArray.data[dataBits];
                }
                else if(dataBits == 26)
                {
                    // 27th data bit
                    group = msgArray.data[dataBits];
                }
                else if(dataBits == 27)
                {
                    // 28th data bit
                    on = msgArray.data[dataBits];
                }
                else
                {
                    // last 4 data bits
                    recipient <<= 1;
                    recipient |= msgArray.data[dataBits];
                }
            }

            struct timeval curTime;
            gettimeofday(&curTime, NULL);

            g_DebugManager->addLog(LogType_Message, "-------- %ds, %dus --------", curTime.tv_sec, curTime.tv_usec);

            g_DebugManager->addLog(LogType_Message, "sender: %d", sender);

            if(group)
            {
                g_DebugManager->addLog(LogType_Message, "group command");
            }
            else
            {
                g_DebugManager->addLog(LogType_Message, "no group");
            }

            if(on)
            {
                g_DebugManager->addLog(LogType_Message, "on");
            }
            else
            {
                g_DebugManager->addLog(LogType_Message, "off");
            }

            g_DebugManager->addLog(LogType_Message, "recipient: %d", recipient);
            g_DebugManager->addLog(LogType_Message, "------------------------------");
            delay(1000);
        }
        else
        {
            if (PLOT_DATA && PLOT_DATA_ON_FAIL)
            {
                g_DebugManager->addLog(LogType_Warning, "No data...");
                plotLastMessage();
            }
        }
    }
}

void RadioManager::sendBit(const bool _bit)
{
    if (_bit)
    {
        Wiring::writeDigital(m_transmitterPin, HIGH);
        delayMicroseconds(SIGNAL_INTER_TIME);
        Wiring::writeDigital(m_transmitterPin, LOW);
        delayMicroseconds(SIGNAL_ONE_TIME);
        Wiring::writeDigital(m_transmitterPin, HIGH);
    }
    else
    {
        Wiring::writeDigital(m_transmitterPin, HIGH);
        delayMicroseconds(SIGNAL_INTER_TIME);
        Wiring::writeDigital(m_transmitterPin, LOW);
        delayMicroseconds(SIGNAL_ZERO_TIME);
        Wiring::writeDigital(m_transmitterPin, HIGH);
    }
}

void RadioManager::sendPair(const bool _bit)
{
    if(_bit)
    {
        //printf("1");
        sendBit(true);
        sendBit(false);
    }
    else
    {
        //printf("0");
        sendBit(false);
        sendBit(true);
    }
}

void RadioManager::transmit(const unsigned int _nbMsg, const bool _intOn, const bool _group, const unsigned int _roomId, const unsigned int _objectId)
{
    if (_nbMsg != 0)
    {
        if (const Object* object = g_ObjectsManager->GetObject(_roomId, _objectId))
        {
            g_DebugManager->addLog(LogType_Message, "Transmitting signal: group(%s), roomId(%d), objectId(%d), state(%s)", _group?"true":"false", _roomId, _objectId, _intOn?"on":"off");

            transmit(_intOn, _group, object->m_groupId, object->m_elementId);
            for (unsigned int i = 1 ; i < _nbMsg ; ++i)
            {
                delay(10);
                transmit(_intOn, _group, object->m_groupId, object->m_elementId);
            }
        }
    }
}

void RadioManager::transmit(const bool _intOn, const bool _group, const unsigned int _groupId, const unsigned int _intId)
{
// Sequence de verrou anoncant le départ du signal au recepeteur
    Wiring::writeDigital(m_transmitterPin, HIGH);
    delayMicroseconds(275);     // un bit de bruit avant de commencer pour remettre les delais du recepteur a 0
    Wiring::writeDigital(m_transmitterPin, LOW);
    delayMicroseconds(9900);     // premier verrou de 9900µs
    Wiring::writeDigital(m_transmitterPin, HIGH);   // high again
    delayMicroseconds(275);      // attente de 275µs entre les deux verrous*/
    Wiring::writeDigital(m_transmitterPin, LOW);    // second verrou de 2675µs
    delayMicroseconds(2675);
    Wiring::writeDigital(m_transmitterPin, HIGH);  // On reviens en état haut pour bien couper les verrous des données

// Envoie du code emetteur

    unsigned int emitterId = _groupId;
    unsigned int testBit = std::pow(2, 25);
    for(int i = 0 ; i < 26 ; ++i)
    {
        sendPair(emitterId & testBit);
        emitterId <<= 1;
    }

// Envoie du bit définissant si c'est une commande de groupe ou non (26em bit)
    sendPair(_group);

// Envoie du bit définissant si c'est allumé ou eteint 27em bit)
    sendPair(_intOn);

// Envoie des 4 derniers bits, qui représentent le code interrupteur, ici 0 (encode sur 4 bit donc 0000)
// nb: sur  les télécommandes officielle chacon, les interrupteurs sont logiquement nommés de 0 à x
// interrupteur 1 = 0 (donc 0000) , interrupteur 2 = 1 (1000) , interrupteur 3 = 2 (0100) etc...
    unsigned int intId = _intId;
    testBit = std::pow(2, 3);
    for(int i = 0 ; i < 4 ; ++i)
    {
        sendPair(intId & testBit);
        intId <<= 1;
    }

    Wiring::writeDigital(m_transmitterPin, HIGH);   // coupure données, verrou
    delayMicroseconds(275);      // attendre 275µs
    Wiring::writeDigital(m_transmitterPin, LOW);    // verrou 2 de 2675µs pour signaler la fermeture du signal

    //printf("\n");
}

}
