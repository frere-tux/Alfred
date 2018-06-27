#include "Task.h"

#include <Requests/RequestsManager.h>

namespace Al
{

Task::Task() : m_id(INVALID_TASK_ID)
{
}

Task::Task(TaskId _id, const TaskParam& _param)
    : m_id(_id)
    , m_param(_param)
    , m_state(TaskState_Ready)
    , m_nextStateChange(0)
{
}

void Task::Start(time_t currentTime)
{
    for (auto& requestParam : m_param.m_startRequests)
    {
        g_RequestsManager->AddRequest(requestParam);
    }

    m_nextStateChange = currentTime + m_param.m_duration;
    m_state = TaskState_Started;
}

void Task::Stop(time_t currentTime)
{
    for (auto& requestParam : m_param.m_endRequests)
    {
        g_RequestsManager->AddRequest(requestParam);
    }

    m_nextStateChange = currentTime - m_param.m_duration + m_param.m_periodicity;
    m_state = TaskState_Stopped;
}

void Task::End()
{
    m_nextStateChange = 0;
    m_state = TaskState_Finished;
}

}
