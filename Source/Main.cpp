#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Radio/Radio.h"
#include "Wiring/Wiring.h"
#include "Managers/DebugManager.h"
#include "Tools/Plot.h"
#include "Tools/StringTools.h"


#define PLOT_DATA true
#define PLOT_DATA_ON_FAIL false


using namespace Al;

void scheduler_realtime()
{
	struct sched_param p;
	p.__sched_priority = sched_get_priority_max(SCHED_RR);

	if( sched_setscheduler( 0, SCHED_RR, &p ) == -1 )
	{
        perror("Failed to switch to realtime scheduler.");
	}
}

void scheduler_standard()
{
	struct sched_param p;
	p.__sched_priority = 0;

	if( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 )
	{
        perror("Failed to switch to normal scheduler.");
	}
}

int main (int argc, char** argv)
{
	scheduler_realtime();

	DebugManager debug;

	// Get radio receptor pin number from arguments
	if (argc != 2)
	{
        return -1;
	}
	unsigned int pin = atoi(argv[1]);


	if (!Wiring::init())
	{
        return -1;
	}

    Wiring::setPinMode(pin, PinMode_Input);

    bool endLoop = false;
    while (!endLoop)
    {
        unsigned long sender = 0;
        bool group = false;
        bool on = false;
        unsigned int recipient = 0;


        debug.addLog(LogType_Message, "Waiting for signal");


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

        Radio radio(2, 99);

        if (radio.tryGetMessage(manchArray, validManchArray, true, PLOT_DATA))
        {
            if(radio.manchesterCheck(manchArray, validManchArray, msgArray, validMsgArray))
            {
                if (PLOT_DATA && !PLOT_DATA_ON_FAIL)
                {
                    radio.plotLastMessage();
                }

                for (unsigned int dataBits = 0 ; dataBits < RADIO_MESSAGE_SIZE ; ++dataBits)
                {
                    if(dataBits < 25)
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

                debug.addLog(LogType_Message, "------------------------------");
                debug.addLog(LogType_Message, "Donnees detectees");

                std::stringstream strSender;
                strSender << "sender: " << sender;
                debug.addLog(LogType_Message, strSender.str().c_str());

                if(group)
                {
                    debug.addLog(LogType_Message, "group command");
                }
                else
                {
                    debug.addLog(LogType_Message, "no group");
                }

                if(on)
                {
                    debug.addLog(LogType_Message, "on");
                }
                else
                {
                    debug.addLog(LogType_Message, "off");
                }

                std::stringstream strRecipient;
                strRecipient << "recipient: " << recipient;
                debug.addLog(LogType_Message, strRecipient.str().c_str());

                delay(1000);
            }
            else
            {
                if (PLOT_DATA && PLOT_DATA_ON_FAIL)
                {
                    radio.plotLastMessage();
                }

                debug.addLog(LogType_Warning, "Aucune donnee...");
            }
        }

        /*std::cout << "Again?" << std::endl;
        char inputChar;
        std::cin >> inputChar;

        if (inputChar != 'y' && inputChar != 'Y')
        {
            endLoop = true;
        }*/
    }

	scheduler_standard();
}
