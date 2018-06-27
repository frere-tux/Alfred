#include "TasksManager.h"

#include <cassert>

#include <Debug/DebugManager.h>

namespace Al
{

static TaskId s_idGenerator = 0;

TasksManager::~TasksManager()
{
    for (auto& taskIt : m_tasks)
    {
        delete taskIt.second;
    }

    m_tasks.clear();
}

void TasksManager::Process(float _dt)
{
    m_tasksMutex.lock();
    std::map<TaskId, Task*> tempTasks(m_tasks);
    m_tasksMutex.unlock();

    time_t currentTime;
    time(&currentTime);

    for (auto& taskIt : tempTasks)
    {
        Task * task =  taskIt.second;

        if (currentTime >= task->GetEndTime())
        {
            g_DebugManager->addLog(LogType_Message, "TaskManager: End task %d", task->GetId());
            if (task->GetState() == TaskState_Started)
            {
                task->Stop(currentTime);
            }

            task->End();
            RemoveTask(task->GetId());
        }
        else if ((task->GetState() == TaskState_Ready && currentTime >= task->GetStartTime())
               || (task->GetState() == TaskState_Stopped && currentTime >= task->GetNextChangeTime()))
        {
            g_DebugManager->addLog(LogType_Message, "TaskManager: Start task %d", task->GetId());
            task->Start(currentTime);
        }
        else if (task->GetState() == TaskState_Started && currentTime >= task->GetNextChangeTime())
        {
            g_DebugManager->addLog(LogType_Message, "TaskManager: Stop task %d", task->GetId());
            task->Stop(currentTime);
        }
    }
}

TaskId TasksManager::AddTask(const TaskParam& _param)
{
    g_DebugManager->addLog(LogType_Message, "TaskManager: Task %d added", s_idGenerator);

    m_tasksMutex.lock();
    assert(m_tasks.find(s_idGenerator) == m_tasks.end());
    m_tasks[s_idGenerator] = new Task(s_idGenerator, _param);
    m_tasksMutex.unlock();

    TaskId id = s_idGenerator++;

    if (s_idGenerator == MAX_U16)
    {
        s_idGenerator = 0;
    }

    return id;
}

void TasksManager::RemoveTask(TaskId _id)
{
    m_tasksMutex.lock();

    auto taskIt = m_tasks.find(_id);

    delete taskIt->second;

    assert(taskIt != m_tasks.end());
    m_tasks.erase(taskIt);

    m_tasksMutex.unlock();
}

}
