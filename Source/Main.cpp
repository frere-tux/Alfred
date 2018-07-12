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

#include <Tasks/TasksManager.h>
#include <Requests/Request.h>

#define LOG_FREQUENCY 60 * 60

using namespace Al;

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


	TaskParam taskParam;

	taskParam.m_startTime = startTime + 1;
	taskParam.m_endTime = taskParam.m_startTime + 1;
	taskParam.m_duration = 1;
	taskParam.m_periodicity = 1000;

	RequestParam request;

	request.m_type = RequestType_Device;
	request.m_action = RequestAction_Activate;
	request.m_roomId = 0;
	request.m_objectId = 1;
	taskParam.m_startRequests.push_back(request);

	request.m_type = RequestType_Device;
	request.m_action = RequestAction_Deactivate;
	request.m_roomId = 0;
	request.m_objectId = 1;
	taskParam.m_endRequests.push_back(request);

    g_TasksManager->AddTask(taskParam);


    /// Start main loop
    //time_t currentTime;
    //currentTime = startTime;
    //time_t nextLogTime = currentTime + LOG_FREQUENCY;

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

    managers.EndManagers();

    g_DebugManager->addLog(LogType_Important, "\n      ===== End of session =====\n\n");

    return 0;
}
