#pragma once

#include "Managers/Manager.h"

namespace Al
{
    enum LogType
    {
        LogType_Message,
        LogType_Warning,
        LogType_Error
    };

    class DebugManager : public Manager
    {
    public:
        DebugManager();
        virtual ~DebugManager() {}

        void addLog(const LogType _type, const char* _message) const;

    private:
        bool    m_active;
        bool    m_displayMessages;
        bool    m_displayWarnings;
        bool    m_displayErrors;
    };
}
