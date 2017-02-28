#include "Managers/ConfigManager.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "Managers/DebugManager.h"

using namespace Al;

Config::Config()
    : m_receptorPin(2)
    , m_emitterPin(7)
    , m_enableDebug(true)
{
}

void Config::Init()
{
    FILE * pFile = fopen ("/home/pi/Projects/Alfred/Data/Config.dat" , "r");

    char buffer[65536];

    rapidjson::FileReadStream fileStream(pFile, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0>(fileStream);

    assert(document.IsObject());

    Debug::getInstance().addLog(LogType_Message, "-------------------CONFIG--------------------");

    const rapidjson::Value& receptorPin = document["receptorPin"];
    if (receptorPin.IsUint())
    {
        m_receptorPin = receptorPin.GetUint();
        Debug::getInstance().addLog(LogType_Message, "receptor pin: %u", m_receptorPin);
    }

    const rapidjson::Value& emitterPin = document["emitterPin"];
    if (emitterPin.IsUint())
    {
        m_emitterPin = emitterPin.GetUint();
        Debug::getInstance().addLog(LogType_Message, "emitter pin: %u", m_emitterPin);
    }

    const rapidjson::Value& enableDebug = document["enableDebug"];
    if (enableDebug.IsBool())
    {
        m_enableDebug = enableDebug.GetBool();
        Debug::getInstance().addLog(LogType_Message, "debug %s", m_enableDebug ? "enabled" : "disabled" );
    }

    Debug::getInstance().addLog(LogType_Message, "---------------------------------------------");
}
