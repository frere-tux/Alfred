#include "DebugManager.h"

#include <iostream>
#include <cstdarg>

#include "Managers/ConfigManager.h"

using namespace Al;

Debug::Debug()
    : m_active(true)
    , m_displayMessages(true)
    , m_displayWarnings(true)
    , m_displayErrors(true)
{
}

void Debug::Init()
{
    m_active = Config::getInstance().GetEnableDebug();
}

void Debug::addLog(const LogType _type, const std::string& _message) const
{
    if ( ( (_type == LogType_Message && m_displayMessages)
       || (_type == LogType_Warning && m_displayWarnings)
       || (_type == LogType_Error && m_displayErrors) )
       && m_active)
    {
        std::cout << _message << std::endl;
    }
}

void Debug::addLog(LogType _type, const char* _format, ...) const
{
    if (!_format)
    {
        return;
    }

    const int tempBufferSize = 1024;
    char tempBuffer[tempBufferSize];
    va_list args;
    va_start(args, _format);

    vsnprintf(tempBuffer, tempBufferSize, _format, args);
    tempBuffer[tempBufferSize-1] = 0;

    va_end(args);

    addLog(_type, std::string(tempBuffer));
}

