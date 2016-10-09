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
        Plot plot;
        plot.init();

        unsigned int dataBits = 0;

		unsigned long t = 0;

        int prevBit = 0;
        int bit = 0;

        unsigned long sender = 0;
        bool group = false;
        bool on = false;
        unsigned int recipient = 0;


        debug.addLog(LogType_Message, "Waiting for signal");



        t = Radio::getPulseIn(pin, LOW, 1000000);
        while(t < 2550 || t > 2800)
        {
            t = Radio::getPulseIn(pin, LOW,1000000);
        }

        plot.logPulse(t);

        while (dataBits < 64)
        {
            t = Radio::getPulseIn(pin, LOW,1000000);

            plot.logPulse(t);

            if(t > 200 && t < 1000)
            {
                bit = 0;
            }
            else if(t > 1000 && t < 2000)
            {
                bit = 1;
            }
            else
            {
                std::string errorMsg("Bad duration for a data bit (");
                //errorMsg += longToString(t);
                errorMsg += ")";
                debug.addLog(LogType_Error, errorMsg.c_str());

                dataBits = 0;
                break;
            }

            if(dataBits % 2 == 1)
            {
                if((prevBit ^ bit) == 0)
                {
                    // must be either 01 or 10, cannot be 00 or 11
                    std::string errorMsg("Manchester check failed (");
                    //errorMsg += longToString(dataBits);
                    errorMsg += ")";
                    debug.addLog(LogType_Error, errorMsg.c_str());

                    dataBits = 0;
                    break;
                }

                if(dataBits < 53)
                {
                    // first 26 data bits
                    sender <<= 1;
                    sender |= prevBit;
                }
                else if(dataBits == 53)
                {
                    // 26th data bit
                    group = prevBit;
                }
                else if(dataBits == 55)
                {
                    // 27th data bit
                    on = prevBit;
                }
                else
                {
                    // last 4 data bits
                    recipient <<= 1;
                    recipient |= prevBit;
                }
            }

            prevBit = bit;

            ++dataBits;
        }

        if(dataBits > 0)
        {

            plot.plotLog();

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
            debug.addLog(LogType_Warning, "Aucune donnee...");
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
