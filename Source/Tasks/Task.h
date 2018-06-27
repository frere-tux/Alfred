#pragma once

#include <vector>
#include <ctime>

#include <Requests/Request.h>

#include "TaskTypes.h"

namespace Al
{
    struct TaskParam
    {
        time_t  m_startTime;
        time_t  m_endTime;
        time_t  m_periodicity;
        time_t  m_duration;
        std::vector<RequestParam> m_startRequests;
        std::vector<RequestParam> m_endRequests;
    };

    class Task
    {
    public:
        Task();
        Task(TaskId _id, const TaskParam& _param);
        virtual ~Task() {}

        void Start(time_t currentTime);
        void Stop(time_t currentTime);
        void End();

        TaskId GetId() const { return m_id; }
        TaskState GetState() const { return m_state; }
        time_t GetStartTime() const { return m_param.m_startTime; }
        time_t GetEndTime() const { return m_param.m_endTime; }
        time_t GetNextChangeTime() const { return m_nextStateChange; }

    private:
        TaskId      m_id;
        TaskParam   m_param;
        TaskState   m_state;
        time_t     m_nextStateChange;

    };
}
