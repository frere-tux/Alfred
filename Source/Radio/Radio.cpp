#include "Radio.h"

#include <sys/time.h>
#include <time.h>
#include <sstream>
#include <cmath>
#include <iostream>

#include "Wiring/Wiring.h"
#include "Managers/DebugManager.h"

#define EMITTER_ID 21321458 //380562

#define PLOT_DATA false
#define PLOT_DATA_ON_FAIL false

#define SIGNAL_ERROR_EPSILON 200

#define SIGNAL_INTER_TIME 275 //310 orinally, but tweaked
#define SIGNAL_ZERO_TIME 400  //310 orinally, but tweaked
#define SIGNAL_ONE_TIME 1225  //1340 orinally, but tweaked

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

    // Wait for latch 2
    t = Radio::getPulseIn(_timeOut);
    while(t < 2000 || t > 3500)
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

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        std::cout << _manchArray.data[dataBits];

        _validArray.data[dataBits] = 1;
    }
    std::cout << std::endl;

    for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE*2 ; ++dataBits)
    {
        std::cout << _validArray.data[dataBits];
    }
    std::cout << std::endl;

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

            if (_validMsgArray.data[(dataBits -1)/2] == 1)
            {
                std::cout << prevBit;
            }
            else
            {
                std::cout << "x";
            }
        }

        prevBit = bit;
        prevValid = valid;
    }
    std::cout << std::endl;
    return returnValue;
}

void Radio::plotLastMessage()
{
    m_plot.plotLog();
}

void Radio::process()
{
    unsigned long sender = 0;
    bool group = false;
    bool on = false;
    unsigned int recipient = 0;


    Debug::getInstance().addLog(LogType_Message, "Waiting for signal");


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

            Debug::getInstance().addLog(LogType_Message, "------------------------------");
            Debug::getInstance().addLog(LogType_Message, "Donnees detectees");

            std::stringstream strSender;
            strSender << "sender: " << sender;
            Debug::getInstance().addLog(LogType_Message, strSender.str().c_str());

            if(group)
            {
                Debug::getInstance().addLog(LogType_Message, "group command");
            }
            else
            {
                Debug::getInstance().addLog(LogType_Message, "no group");
            }

            if(on)
            {
                Debug::getInstance().addLog(LogType_Message, "on");
            }
            else
            {
                Debug::getInstance().addLog(LogType_Message, "off");
            }

            std::stringstream strRecipient;
            strRecipient << "recipient: " << recipient;
            Debug::getInstance().addLog(LogType_Message, strRecipient.str().c_str());

            delay(1000);
        }
        else
        {
            if (PLOT_DATA && PLOT_DATA_ON_FAIL)
            {
                plotLastMessage();
            }

            Debug::getInstance().addLog(LogType_Warning, "Aucune donnee...");
        }
    }
}

void Radio::sendBit(const bool _bit)
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

void Radio::sendPair(const bool _bit)
{
    if(_bit)
    {
        printf("1");
        sendBit(true);
        sendBit(false);
    }
    else
    {
        printf("0");
        sendBit(false);
        sendBit(true);
    }
}

void Radio::transmit(const unsigned int _nbMsg, const bool _intOn, const bool _group, const unsigned int _intId)
{
    if (_nbMsg != 0)
    {
        transmit(_intOn, _group, _intId);
        for (unsigned int i = 1 ; i < _nbMsg ; ++i)
        {
            delay(10);
            transmit(_intOn, _group, _intId);
        }
    }
}

void Radio::transmit(const bool _intOn, const bool _group, const unsigned int _intId)
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

    unsigned int emitterId = EMITTER_ID;
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

    printf("\n");
}
