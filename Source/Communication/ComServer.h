#include "./gen-cpp/AlfredCommunication.h"

using namespace AlCom;

namespace Al
{
    class RadioManager;

    class CommunicationHandler : virtual public AlfredCommunicationIf
    {
    public:
        CommunicationHandler(RadioManager* _radio);
        void sendSimpleRequest(const SimpleRequest& request);

        static int startCommunicationServer(RadioManager* _radio);

    private:
        RadioManager* m_radio;
    };
}
