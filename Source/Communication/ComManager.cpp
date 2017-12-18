#include "ComManager.h"

#include <thread>

#include "ComServer.h"

namespace Al
{

ComManager::ComManager() : m_comServerThread(nullptr)
{
}

ComManager::~ComManager()
{
    if (m_comServerThread)
    {
        delete m_comServerThread;
        m_comServerThread = nullptr;
    }
}

void ComManager::Process(float _dt)
{
    if (!m_comServerThread)
    {
        m_comServerThread = new std::thread(CommunicationHandler::startCommunicationServer, g_RadioManager);
    }
}

}
