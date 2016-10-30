#include "./gen-cpp/AlfredCommunication.h"

using namespace AlCom;

namespace Al
{
    class Radio;

    class CommunicationHandler : virtual public AlfredCommunicationIf
    {
    public:
        CommunicationHandler(Radio* _radio);
        void sendSimpleRequest(const SimpleRequest& request);

        static int startCommunicationServer(Radio* _radio);

    private:
        Radio* m_radio;
    };
}
