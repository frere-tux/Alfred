#pragma once

#include <Managers/Manager.h>
#include "AlTypes.h"

namespace Al
{
    class ConfigManager : public Manager
    {
    public:
        ConfigManager();
        virtual ~ConfigManager() {}

        virtual void Init() override;

        u8 GetReceptorPin() const { return m_receptorPin; }
        u8 GetEmitterPin() const { return m_emitterPin; }
        bool GetEnableDebug() const { return m_enableDebug; }
        u8 GetTransmissionRedundancy() const { return m_transmissionRedundancy; }

    private:
        u8      m_receptorPin;
        u8      m_emitterPin;
        bool    m_enableDebug;

        u8      m_transmissionRedundancy;
    };
}
