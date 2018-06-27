#pragma once

#include "AlTypes.h"

namespace Al
{

    #define INVALID_TASK_ID MAX_U16
    typedef u16 TaskId;

    enum TaskState
    {
        TaskState_Ready,
        TaskState_Started,
        TaskState_Stopped,
        TaskState_Finished
    };
}
