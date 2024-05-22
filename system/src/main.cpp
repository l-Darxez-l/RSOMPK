#include <Ice/Ice.h>
#include "SIPImpl.h"
#include "StopImpl.h"
#include "LineImpl.h"
#include <memory>

class Server : public Ice::Application
{
public:

    virtual int run(int argc, char* argv[]) override {
        shutdownOnInterrupt();

        Ice::Current ic = Ice::Current{};

        auto communicator = this->communicator();

        auto adapter = communicator->createObjectAdapter("SystemAdapter");

        auto sipServant = new SIPImpl;

        adapter->add(sipServant, Ice::stringToIdentity("SIP"));

        //Tworzenie linii i przystanków

            //Linie
        auto line1 = new LineImpl(1);
        auto line2 = new LineImpl(2);
        auto line3 = new LineImpl(3);

        auto linePrx1 = MPK::LinePrx::uncheckedCast(adapter->addWithUUID(line1));
        auto linePrx2 = MPK::LinePrx::uncheckedCast(adapter->addWithUUID(line2));
        auto linePrx3 = MPK::LinePrx::uncheckedCast(adapter->addWithUUID(line3));

        sipServant->addLine(linePrx1, ic);
        sipServant->addLine(linePrx2, ic);
        sipServant->addLine(linePrx3, ic);

            //Przystanki
        auto stop1 = new StopImpl(1, "Retkinia");
        auto stop2 = new StopImpl(2, "Zdrowie");
        auto stop3 = new StopImpl(3, "Centralna");
        auto stop4 = new StopImpl(4, "Piotrkowska Centrum");
        auto stop5 = new StopImpl(5, "Plac Wolności");
        auto stop6 = new StopImpl(6, "Manufaktura");
        auto stop7 = new StopImpl(7, "Teofilów");
        auto stop8 = new StopImpl(8, "Radogoszcz");
        auto stop9 = new StopImpl(9, "Plac Niepodległości");
        auto stop10 = new StopImpl(10, "Strykowska");
        

        auto stopPrx1 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop1));
        auto stopPrx2 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop2));
        auto stopPrx3 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop3));
        auto stopPrx4 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop4));
        auto stopPrx5 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop5));
        auto stopPrx6 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop6));
        auto stopPrx7 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop7));
        auto stopPrx8 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop8));
        auto stopPrx9 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop9));
        auto stopPrx10 = MPK::StopPrx::uncheckedCast(adapter->addWithUUID(stop10));

        sipServant->addStop(stopPrx1);
        sipServant->addStop(stopPrx2);
        sipServant->addStop(stopPrx3);
        sipServant->addStop(stopPrx4);
        sipServant->addStop(stopPrx5);
        sipServant->addStop(stopPrx6);
        sipServant->addStop(stopPrx7);
        sipServant->addStop(stopPrx8);
        sipServant->addStop(stopPrx9);
        sipServant->addStop(stopPrx10);

            //Przypisywanie przystanków do linii
        line1->addStop(stopPrx1);
        line1->addStop(stopPrx2);
        line1->addStop(stopPrx3);
        line1->addStop(stopPrx4);
        line1->addStop(stopPrx5);
        line1->addStop(stopPrx6);

        line2->addStop(stopPrx7);
        line2->addStop(stopPrx8);
        line2->addStop(stopPrx3);
        line2->addStop(stopPrx4);
        line2->addStop(stopPrx5);
        line2->addStop(stopPrx10);

        line3->addStop(stopPrx1);
        line3->addStop(stopPrx3);
        line3->addStop(stopPrx4);
        line3->addStop(stopPrx5);
        line3->addStop(stopPrx9);
        line3->addStop(stopPrx10);

            //Przypisywanie linii do przystanków
        stop1->addLine(linePrx1);
        stop1->addLine(linePrx3);
        stop2->addLine(linePrx1);
        stop3->addLine(linePrx1);
        stop3->addLine(linePrx2);
        stop3->addLine(linePrx3);
        stop4->addLine(linePrx1);
        stop4->addLine(linePrx2);
        stop4->addLine(linePrx3);
        stop5->addLine(linePrx1);
        stop5->addLine(linePrx2);
        stop5->addLine(linePrx3);
        stop6->addLine(linePrx1);
        stop7->addLine(linePrx2);
        stop8->addLine(linePrx2);
        stop9->addLine(linePrx3);
        stop10->addLine(linePrx2);
        stop10->addLine(linePrx3);

        

        adapter->activate();

        Ice::LoggerPtr logger = communicator->getLogger();
        logger->trace("ServerLog", "Server is activated and waiting for requests.");

        communicator->waitForShutdown();

        logger->trace("ServerLog", "Server is shutting down.");

        return EXIT_SUCCESS;
    }

};



int main(int argc, char* argv[]) {
    Server app;
    return app.main(argc, argv, "system/server.config");
}