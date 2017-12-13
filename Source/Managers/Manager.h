#pragma once

#include <vector>

#define DECLARE_MANAGER(ManagerClass) class ManagerClass; extern ManagerClass* g_##ManagerClass;

namespace Al
{
    class Manager
    {
    public:
        Manager() {}
        virtual ~Manager() {}

        virtual void    Init() {}
        virtual void    Process(float _dt) {}
        virtual void    End() {}
    };

    class Managers
    {
    public:
        Managers() {}
        ~Managers() {}

        void CreateManagers();
        void InitManagers();
        void ProcessManagers();
        void EndManagers();

    protected:
        std::vector<Manager*> m_managers;
    };

    DECLARE_MANAGER(ConfigManager)
    DECLARE_MANAGER(DebugManager)
    DECLARE_MANAGER(ObjectsManager)
    DECLARE_MANAGER(RadioManager)
}

