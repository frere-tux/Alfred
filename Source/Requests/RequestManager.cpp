#include "RequestsManager.h"

#include <cassert>

namespace Al
{

static RequestId s_idGenerator = 0;

void RequestsManager::Process(float _dt)
{
    for (auto& request : m_requests)
    {
        request.second.Execute();
    }

    m_requests.clear();
}

RequestId RequestsManager::AddRequest(const RequestParam& _param)
{
    assert(m_requests.find(s_idGenerator) == m_requests.end());

    m_requests[s_idGenerator] = Request(s_idGenerator, _param);

    ++s_idGenerator;

    if (s_idGenerator == MAX_U16)
    {
        s_idGenerator = 0;
    }

    return m_requests[s_idGenerator].GetId();
}

void RequestsManager::RemoveRequest(RequestId _id)
{
    auto requestIt = m_requests.find(_id);

    assert(requestIt != m_requests.end());

    m_requests.erase(requestIt);
}

}
