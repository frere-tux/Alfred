#include "RequestsManager.h"

#include <cassert>

#include <Debug/DebugManager.h>

namespace Al
{

static RequestId s_idGenerator = 0;

void RequestsManager::Process(float _dt)
{
    m_requestsMutex.lock();
    std::map<RequestId, Request> tempRequests(m_requests);
    m_requests.clear();
    m_requestsMutex.unlock();

    for (auto& request : tempRequests)
    {
        request.second.Execute();
    }
}

RequestId RequestsManager::AddRequest(const RequestParam& _param)
{
    g_DebugManager->addLog(LogType_Message, "RequestManager: Request %d added", s_idGenerator);

    m_requestsMutex.lock();
    assert(m_requests.find(s_idGenerator) == m_requests.end());
    m_requests[s_idGenerator] = Request(s_idGenerator, _param);
    m_requestsMutex.unlock();

    RequestId id = s_idGenerator++;

    if (s_idGenerator == MAX_U16)
    {
        s_idGenerator = 0;
    }

    return id;
}

void RequestsManager::RemoveRequest(RequestId _id)
{
    m_requestsMutex.lock();

    auto requestIt = m_requests.find(_id);

    assert(requestIt != m_requests.end());

    m_requests.erase(requestIt);

    m_requestsMutex.unlock();
}

}
