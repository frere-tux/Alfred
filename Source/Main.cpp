#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include "Radio/Radio.h"
#include "Wiring/Wiring.h"
#include "Managers/DebugManager.h"
#include "Tools/StringTools.h"
#include "Communication/ComServer.h"


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

    if (!Wiring::init())
	{
        return -1;
	}

	if (argc != 3)
	{
        return -1;
	}

	// Get radio receptor pin number from arguments
	unsigned int receptorPin = atoi(argv[1]);
	Wiring::setPinMode(receptorPin, PinMode_Input);

	// Get radio emitter pin number from arguments
	unsigned int emitterPin = atoi(argv[2]);
	Wiring::setPinMode(emitterPin, PinMode_Output);

	Radio radio(receptorPin, emitterPin);

	pid_t pid = fork();

    bool endLoop = false;
    while (!endLoop)
    {
        if (pid == 0)
        {
            radio.process();
        }
        else if (pid > 0)
        {
            CommunicationHandler::startCommunicationServer(&radio);
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
