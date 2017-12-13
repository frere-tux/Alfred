#include "DebugManager.h"

#include <iostream>
#include <cstdarg>
#include <fstream>

#include <Config/ConfigManager.h>

#define LOG_TO_FILE 0

namespace Al
{

DebugManager::DebugManager()
    : m_active(true)
    , m_displayMessages(true)
    , m_displayWarnings(true)
    , m_displayErrors(true)
    , m_displayImportant(true)
{
}

void DebugManager::Init()
{
    m_active = g_ConfigManager->GetEnableDebug();
}

void DebugManager::addLog(const LogType _type, const std::string& _message) const
{
    if ( ( (_type == LogType_Message && m_displayMessages)
       || (_type == LogType_Warning && m_displayWarnings)
       || (_type == LogType_Error && m_displayErrors)
       || (_type == LogType_Important && m_displayImportant) )
       && m_active)
    {
        std::cout << _message << std::endl;

        if (LOG_TO_FILE && (_type == LogType_Error || _type == LogType_Important))
        {
            std::ofstream out;
            out.open("/home/pi/Projects/Alfred/Data/Alfred.log", std::ios::app);
            out << _message;
            out.close();
        }
    }
}

void DebugManager::addLog(LogType _type, const char* _format, ...) const
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

}
