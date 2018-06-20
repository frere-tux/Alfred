#include "./gen-cpp/AlfredCommunication.h"

using namespace AlCom;

namespace Al
{
    class CommunicationHandler : virtual public AlfredCommunicationIf
    {
    public:
        CommunicationHandler();
        void sendSimpleRequest(const SimpleRequest& request);

        static int startCommunicationServer();
    };
}
