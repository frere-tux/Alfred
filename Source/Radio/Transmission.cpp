#include "Transmission.h"

#include <Config/ConfigManager.h>

namespace Al
{

Transmission::Transmission() : m_id(INVALID_TRANSMISSION_ID)
{
}

Transmission::Transmission(TransmissionId _id, const TransmissionParam& _param)
    : m_id(_id)
    , m_param(_param)
{
}

}
