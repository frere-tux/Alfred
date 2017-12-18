#pragma once

#include "AlTypes.h"

namespace Al
{

    #define INVALID_REQUEST_ID MAX_U16
    typedef u16 RequestId;

    enum RequestType
    {
        RequestType_Device,
        RequestType_Script,
        RequestType_Task
    };

    enum RequestAction
    {
        RequestAction_Add,
        RequestAction_Remove,
        RequestAction_Activate,
        RequestAction_Deactivate,
        RequestAction_Switch,
        RequestAction_Set
    };
}
