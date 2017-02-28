#include "Manager.h"
#include "Managers/ConfigManager.h"
#include "Managers/DebugManager.h"
#include "Managers/ObjectsManager.h"

using namespace Al;

void Manager::InitManagers()
{
    Config::getInstance().Init();
    Debug::getInstance().Init();
    ObjectsManager::getInstance().Init();
}
