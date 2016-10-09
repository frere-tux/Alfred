#include "DebugManager.h"

#include <iostream>

using namespace Al;

DebugManager::DebugManager()
    : Manager()
    , m_active(true)
    , m_displayMessages(true)
    , m_displayWarnings(true)
    , m_displayErrors(true)
{
}

void DebugManager::addLog(const LogType _type, const char* _message) const
{
    if ( ( (_type == LogType_Message && m_displayMessages)
       || (_type == LogType_Warning && m_displayWarnings)
       || (_type == LogType_Error && m_displayErrors) )
       && m_active)
    {
        std::cout << _message << std::endl;
    }
}
