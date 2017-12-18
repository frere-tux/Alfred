#pragma once

#include <Managers/Manager.h>

namespace std{ class thread; }

namespace Al
{
    class ComManager : public Manager
    {
    public:
        ComManager();
        virtual ~ComManager();

        virtual void   Process(float _dt) override;

    private:
        std::thread* m_comServerThread;
    };
}
