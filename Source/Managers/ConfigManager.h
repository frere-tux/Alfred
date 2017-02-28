#pragma once

#include "Managers/Manager.h"
#include "AlTypes.h"

namespace Al
{
    class Config : public Manager
    {
    protected:
        Config();

    public:
        static Config& getInstance()
        {
            static Config instance;
            return instance;
        }

        ~Config() {}

        Config(const Config&) = delete;
        void operator=(const Config&) = delete;

        void Init();

        u8 GetReceptorPin() const { return m_receptorPin; }
        u8 GetEmitterPin() const { return m_emitterPin; }
        bool GetEnableDebug() const { return m_enableDebug; }

    private:
        u8      m_receptorPin;
        u8      m_emitterPin;
        bool    m_enableDebug;
    };
}
