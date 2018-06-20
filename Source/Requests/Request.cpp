#include "Request.h"

#include <Objects/ObjectsManager.h>
#include <Radio/RadioManager.h>
#include <Debug/DebugManager.h>

namespace Al
{

Request::Request() : m_id(INVALID_REQUEST_ID)
{
}

Request::Request(RequestId _id, const RequestParam& _param)
    : m_id(_id)
    , m_param(_param)
{
}

bool Request::Execute()
{

    g_DebugManager->addLog(LogType_Message, "Execute request: room %d, element %d", m_param.m_roomId, m_param.m_objectId);

    switch (m_param.m_type)
    {
    case RequestType_Device:

        switch (m_param.m_action)
        {
        case RequestAction_Activate:
        case RequestAction_Deactivate:
            if (const Object * object = g_ObjectsManager->GetObject(m_param.m_roomId, m_param.m_objectId))
            {
                g_RadioManager->transmit(3, m_param.m_action == RequestAction_Activate ? true : false, false, object->m_groupId, object->m_elementId);
                return true;
            }
            else
            {
                return false;
            }

        default:
            return false;
        }

    case RequestType_DeviceGroup:
        switch (m_param.m_action)
        {
        case RequestAction_Activate:
        case RequestAction_Deactivate:
            if (const Object * object = g_ObjectsManager->GetObject(m_param.m_roomId, m_param.m_objectId))
            {
                g_RadioManager->transmit(3, m_param.m_action == RequestAction_Activate ? true : false, true, object->m_groupId, object->m_elementId);
                return true;
            }
            else
            {
                return false;
            }

        default:
            return false;
        }

    case RequestType_Script:
    case RequestType_Task:
    default:
        return false;
    }
}

}
