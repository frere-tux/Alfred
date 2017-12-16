#pragma once

#include <map>

#include <Managers/Manager.h>
#include "Request.h"
#include "RequestsTypes.h"

namespace Al
{
    class RequestsManager : public Manager
    {
    public:
        RequestsManager() {}
        virtual ~RequestsManager() {}

        virtual void   Process(float _dt) override;

        RequestId       AddRequest(const RequestParam& _param);
        void            RemoveRequest(RequestId _id);

    private:
        std::map<RequestId, Request> m_requests;
    };
}
