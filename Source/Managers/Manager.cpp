#include "Manager.h"
#include <Config/ConfigManager.h>
#include <Debug/DebugManager.h>
#include <Objects/ObjectsManager.h>
#include <Radio/RadioManager.h>
#include <Requests/RequestsManager.h>
#include <Communication/ComManager.h>
#include <Tasks/TasksManager.h>

namespace Al
{

#define REGISTER_MANAGER(ManagerClass)  ManagerClass* g_##ManagerClass = nullptr;
#define CREATE_MANAGER(ManagerClass)    g_##ManagerClass = new ManagerClass(); m_managers.push_back(g_##ManagerClass);
#define NULLIFY_MANAGER(ManagerClass)   g_##ManagerClass = nullptr;
#define INIT_MANAGER(ManagerClass)      g_##ManagerClass->Init();
#define PROCESS_MANAGER(ManagerClass)   g_##ManagerClass->Process(0.0f);
#define END_MANAGER(ManagerClass)       g_##ManagerClass->End();

REGISTER_MANAGER(ConfigManager)
REGISTER_MANAGER(DebugManager)
REGISTER_MANAGER(ObjectsManager)
REGISTER_MANAGER(RadioManager)
REGISTER_MANAGER(RequestsManager)
REGISTER_MANAGER(ComManager)
REGISTER_MANAGER(TasksManager)

void Managers::CreateManagers()
{
    CREATE_MANAGER(ConfigManager)
    CREATE_MANAGER(DebugManager)
    CREATE_MANAGER(ObjectsManager)
    CREATE_MANAGER(RequestsManager)
    CREATE_MANAGER(TasksManager)
    CREATE_MANAGER(RadioManager)
    CREATE_MANAGER(ComManager)
}

void Managers::InitManagers()
{
    INIT_MANAGER(ConfigManager)
    INIT_MANAGER(DebugManager)
    INIT_MANAGER(ObjectsManager)
    INIT_MANAGER(RadioManager)
}

void Managers::ProcessManagers()
{
    PROCESS_MANAGER(ComManager)
    PROCESS_MANAGER(TasksManager)
    PROCESS_MANAGER(RequestsManager)
    PROCESS_MANAGER(RadioManager)
}

void Managers::EndManagers()
{
    END_MANAGER(RadioManager)

    for (auto manager : m_managers)
    {
        delete manager;
    }

    m_managers.clear();

    NULLIFY_MANAGER(ConfigManager)
    NULLIFY_MANAGER(DebugManager)
    NULLIFY_MANAGER(ObjectsManager)
    NULLIFY_MANAGER(RequestsManager)
    NULLIFY_MANAGER(RadioManager)
    NULLIFY_MANAGER(ComManager)
    NULLIFY_MANAGER(TasksManager)
}

}
