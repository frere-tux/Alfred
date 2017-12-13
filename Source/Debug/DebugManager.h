#pragma once
#pragma once

#include <Managers/Manager.h>

#include <string>

namespace Al
{
    enum LogType
    {
        LogType_Message,
        LogType_Warning,
        LogType_Error,
        LogType_Important
    };

    class DebugManager : public Manager
    {
    public:
        DebugManager();
        virtual ~DebugManager() {}

        virtual void Init() override;

        void activate(const bool _active) { m_active = _active; };
        void addLog(const LogType _type, const std::string& _message) const;
        void addLog(LogType _type, const char* _format, ...) const;

    private:
        bool    m_active;
        bool    m_displayMessages;
        bool    m_displayWarnings;
        bool    m_displayErrors;
        bool    m_displayImportant;
    };
}
