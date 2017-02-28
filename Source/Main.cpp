#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include "Managers/Manager.h"
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
    Manager::InitManagers();

    if (!Wiring::init())
	{
        return -1;
	}

	Radio radio;
	radio.Init();

	pid_t pid = fork();

    bool endLoop = false;
    while (!endLoop)
    {
        if (pid == 0)
        {
            scheduler_realtime();
            radio.process();
            scheduler_standard();
        }
        else if (pid > 0)
        {
            CommunicationHandler::startCommunicationServer(&radio);
        }
    }
}
