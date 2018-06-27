#include "Request.h"

#include <Objects/ObjectsManager.h>
#include <Radio/RadioManager.h>
#include <Debug/DebugManager.h>
#include <Config/ConfigManager.h>

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
    case RequestType_DeviceGroup:
        switch (m_param.m_action)
        {
        case RequestAction_Activate:
        case RequestAction_Deactivate:

            if (const Object * object = g_ObjectsManager->GetObject(m_param.m_roomId, m_param.m_objectId))
            {
                printf("room: %d - object: %d\n", object->m_groupId, object->m_elementId);
                TransmissionParam transmission;
                transmission.m_state = m_param.m_action == RequestAction_Activate ? true : false;
                transmission.m_group = m_param.m_type == RequestType_DeviceGroup ? true : false;
                transmission.m_roomId = object->m_groupId;
                transmission.m_objectId = object->m_elementId;

                g_RadioManager->AddTransmission(transmission);

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
