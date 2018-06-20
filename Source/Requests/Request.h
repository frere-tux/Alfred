#pragma once

#include "RequestsTypes.h"

namespace Al
{
    struct RequestParam
    {
        RequestType     m_type;
        RequestAction   m_action;
        u8              m_roomId;
        u8              m_objectId;
    };

    class Request
    {
    public:
        Request();
        Request(RequestId _id, const RequestParam& _param);
        virtual ~Request() {}

        bool Execute();

        RequestId GetId() const { return m_id; }

    private:
        RequestId       m_id;
        RequestParam    m_param;
    };
}
