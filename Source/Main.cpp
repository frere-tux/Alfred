#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <Managers/Manager.h>
#include <Radio/RadioManager.h>
#include <Wiring/Wiring.h>
#include <Debug/DebugManager.h>
#include <Tools/StringTools.h>
#include <Communication/ComServer.h>

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
    /// Managers initialization
    Managers managers;
    managers.CreateManagers();
    managers.InitManagers();

    /// WiringPi initialization
    if (!Wiring::init())
	{
        return -1;
	}

    /// Log session start
    time_t startTime;

    struct tm * timeinfo;
    char dateBuffer[128];

    time(&startTime);

    timeinfo = localtime(&startTime);

    strftime(dateBuffer,sizeof(dateBuffer),"%d-%m-%Y %I:%M:%S",timeinfo);

	g_DebugManager->addLog(LogType_Important, "\n\n      ===== New Session (%s) =====\n", dateBuffer);

    /// Start main loop
    //time_t currentTime;
    //currentTime = startTime;
    //time_t nextLogTime = currentTime + LOG_FREQUENCY;

	pid_t pid = fork();

    if (pid == 0)
    {
        scheduler_realtime();

        bool endLoop = false;
        while (!endLoop)
        {
            /*time(&currentTime);
            if (currentTime >= nextLogTime)
            {
                long int duration = currentTime - startTime;
                g_DebugManager->addLog(LogType_Important, "-- online for %ld minutes\n", duration/60);

                nextLogTime += LOG_FREQUENCY;
            }*/

            managers.ProcessManagers();
        }

        scheduler_standard();
    }
	else if (pid > 0)
    {
        CommunicationHandler::startCommunicationServer(g_RadioManager);
    }

    g_DebugManager->addLog(LogType_Important, "\n      ===== End of session =====\n\n");

    return 0;
}
