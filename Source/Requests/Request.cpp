#include "Request.h"

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
    return true;
}

}
