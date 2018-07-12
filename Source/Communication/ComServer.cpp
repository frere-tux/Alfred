#include "ComServer.h"

#include <time.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <wiringPi.h>

#include <Requests/RequestsManager.h>
#include <Objects/ObjectsManager.h>
#include <Debug/DebugManager.h>
#include <Tasks/TasksManager.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
using namespace AlCom;

namespace Al
{

CommunicationHandler::CommunicationHandler()
{
    std::cout << "Communication server online" << std::endl;
}

void CommunicationHandler::sendSimpleRequest(const SimpleRequest& request)
{
    g_DebugManager->addLog(LogType_Message, "Simple request received: group(%s), roomId(%d), objectId(%d), state(%s)", request.group?"true":"false", request.groupID, request.elementID, request.state?"on":"off");

    RequestParam param;
    param.m_roomId = request.groupID;
    param.m_objectId = request.elementID;
    param.m_type = request.group ? RequestType_DeviceGroup : RequestType_Device;
    param.m_action = request.state ? RequestAction_Activate : RequestAction_Deactivate;

    g_RequestsManager->AddRequest(param);
}

void CommunicationHandler::sendTask(const ComTask& task)
{
    /*g_DebugManager->addLog(LogType_Debug, "Task: startTime(%ld) - endTime(%ld) - duration(%ld) - periodicity(%ld)", startTime, endTime, duration, periodicity);

    g_DebugManager->addLog(LogType_Debug, "  Start requests:");
    for (auto& request : task.startRequests)
    {
        g_DebugManager->addLog(LogType_Debug, "    Room(%d) - Object(%d) - Groupe(%s) - State(%s)", request.groupID, request.elementID, request.group ? "true" : "false", request.state ? "on" : "off");
    }

    g_DebugManager->addLog(LogType_Debug, "  End requests:");
    for (auto& request : task.endRequests)
    {
        g_DebugManager->addLog(LogType_Debug, "    Room(%d) - Object(%d) - Groupe(%s) - State(%s)", request.groupID, request.elementID, request.group ? "true" : "false", request.state ? "on" : "off");
    }*/

    TaskParam taskParam;
    taskParam.m_startTime = (time_t)task.startTime;
    taskParam.m_endTime = (time_t)task.endTime;
    taskParam.m_duration = (time_t)task.duration;
    taskParam.m_periodicity = (time_t)task.periodicity;

    for (auto& request : task.startRequests)
    {
         RequestParam requestParam;
         requestParam.m_roomId = request.groupID;
         requestParam.m_objectId = request.elementID;
         requestParam.m_type = request.group ? RequestType_DeviceGroup : RequestType_Device;
         requestParam.m_action = request.state ? RequestAction_Activate : RequestAction_Deactivate;

         taskParam.m_startRequests.push_back(requestParam);
    }

    for (auto& request : task.endRequests)
    {
         RequestParam requestParam;
         requestParam.m_roomId = request.groupID;
         requestParam.m_objectId = request.elementID;
         requestParam.m_type = request.group ? RequestType_DeviceGroup : RequestType_Device;
         requestParam.m_action = request.state ? RequestAction_Activate : RequestAction_Deactivate;

         taskParam.m_endRequests.push_back(requestParam);
    }

    g_TasksManager->AddTask(taskParam);
}

int CommunicationHandler::startCommunicationServer()
{
    int port = 9090;
    shared_ptr<CommunicationHandler> handler(new CommunicationHandler());
    shared_ptr<TProcessor> processor(new AlfredCommunicationProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

}

