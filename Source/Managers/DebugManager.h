#pragma once
#pragma once

#include "Managers/Manager.h"

#include <string>

namespace Al
{
    enum LogType
    {
        LogType_Message,
        LogType_Warning,
        LogType_Error
    };

    class Debug
    {
    private:
        Debug();

    public:
        static Debug& getInstance()
        {
            static Debug instance;
            return instance;
        }

        virtual ~Debug() {}

        Debug(const Debug&) = delete;
        void operator=(const Debug&) = delete;

        void Init();

        void activate(const bool _active) { m_active = _active; };
        void addLog(const LogType _type, const std::string& _message) const;
        void addLog(LogType _type, const char* _format, ...) const;

    private:
        bool    m_active;
        bool    m_displayMessages;
        bool    m_displayWarnings;
        bool    m_displayErrors;
    };
}
