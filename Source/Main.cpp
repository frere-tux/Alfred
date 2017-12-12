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

#define LOG_FREQUENCY 60 * 60

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
    // Managers initialization
    Manager::InitManagers();

    // WiringPi initialization
    if (!Wiring::init())
	{
        return -1;
	}

    // Radio service initialization
	Radio radio;
	radio.Init();

    // Log session start
    time_t startTime;

    struct tm * timeinfo;
    char dateBuffer[128];

    time(&startTime);

    timeinfo = localtime(&startTime);

    strftime(dateBuffer,sizeof(dateBuffer),"%d-%m-%Y %I:%M:%S",timeinfo);

	Debug::getInstance().addLog(LogType_Important, "\n\n      ===== New Session (%s) =====\n", dateBuffer);

    // Start main loop
    time_t currentTime;
    currentTime = startTime;

    time_t nextLogTime = currentTime + LOG_FREQUENCY;

	pid_t pid = fork();

    if (pid == 0)
    {
        scheduler_realtime();
    }
	else if (pid > 0)
    {
        CommunicationHandler::startCommunicationServer(&radio);
    }

    bool endLoop = false;
    while (!endLoop)
    {
        if (pid == 0)
        {
            time(&currentTime);
            if (currentTime >= nextLogTime)
            {
                long int duration = currentTime - startTime;
                Debug::getInstance().addLog(LogType_Important, "-- online for %ld minutes\n", duration/60);

                nextLogTime += LOG_FREQUENCY;
            }

            radio.process();
        }
        else if (pid > 0)
        {

        }
    }

    if (pid == 0)
    {
        scheduler_standard();
    }

    Debug::getInstance().addLog(LogType_Important, "\n      ===== End of session =====\n\n");
}
