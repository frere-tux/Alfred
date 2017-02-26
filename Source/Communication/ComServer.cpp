#include "ComServer.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <wiringPi.h>

#include "Radio/Radio.h"
#include "Managers/DebugManager.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
using namespace AlCom;
using namespace Al;


CommunicationHandler::CommunicationHandler(Radio* _radio)
    : m_radio(_radio)
{
    std::cout << "Communication server online" << std::endl;
}

void CommunicationHandler::sendSimpleRequest(const SimpleRequest& request)
{
    if (m_radio)
    {
        Debug::getInstance().addLog(LogType_Message, "Simple request received: group(%s), roomId(%d), objectId(%d), state(%s)", request.group?"true":"false", request.groupID, request.elementID, request.state?"on":"off");

        m_radio->transmit(3, request.state, request.group, request.groupID, request.elementID);
    }
}

int CommunicationHandler::startCommunicationServer(Radio* _radio)
{
    int port = 9090;
    shared_ptr<CommunicationHandler> handler(new CommunicationHandler(_radio));
    shared_ptr<TProcessor> processor(new AlfredCommunicationProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

