#pragma once

#include "RadioTypes.h"

namespace Al
{
    struct TransmissionParam
    {
        bool    m_state;
        bool    m_group;
        u32      m_roomId;
        u32      m_objectId;
    };

    class Transmission
    {
    public:
        Transmission();
        Transmission(TransmissionId _id, const TransmissionParam& _param);
        virtual ~Transmission() {}


        TransmissionId GetId() const { return m_id; }
        const TransmissionParam& GetParam() const { return m_param; }

    private:
        TransmissionId       m_id;
        TransmissionParam    m_param;
    };
}
