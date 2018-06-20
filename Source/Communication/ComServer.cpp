#include "ComServer.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <wiringPi.h>

#include <Requests/RequestsManager.h>
#include <Objects/ObjectsManager.h>
#include <Debug/DebugManager.h>

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

