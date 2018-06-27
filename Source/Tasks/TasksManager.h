#pragma once

#include <map>
#include <mutex>

#include <Managers/Manager.h>
#include "Task.h"

namespace Al
{
    class TasksManager : public Manager
    {
    public:
        TasksManager() {}
        virtual ~TasksManager();

        virtual void   Process(float _dt) override;

        TaskId  AddTask(const TaskParam& _param);
        void    RemoveTask(TaskId _id);

    private:
        std::map<TaskId, Task*> m_tasks;

        std::mutex m_tasksMutex;
    };
}
