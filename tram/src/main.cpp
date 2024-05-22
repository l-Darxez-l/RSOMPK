#include <Ice/Ice.h>
#include <fstream>
#include <csignal>
#include "TramImpl.h"
#include "MPK.h" 

void printLinesAndStops(const MPK::SIPPrx& sipProxy) {

    auto lines = sipProxy->getLines();

    for (size_t i = 0; i < lines.size(); i++)
    {
        std::cout << "__________Stops for line: " + std::to_string((i + 1)) + "__________" << std::endl;
        auto lineStops = lines.at(i)->getStops();
        for (size_t j = 0; j < lineStops.size(); j++)
        {
            auto stopID = lineStops.at(j)->getID();
            auto stopName = lineStops.at(j)->getName();
            std::cout << "Stop ID-NAME: " + std::to_string(stopID) + "-" + stopName << std::endl;
        }

        std::cout << "______________________________________" << std::endl;
        
    }
    std::cout << std::endl << std::endl;
}

void addTramToLine(const MPK::SIPPrx& sipProxy, const Ice::ObjectAdapterPtr& adapter, MPK::schedule& schedule, MPK::TramPrx& newTram, MPK::LinePrx& lineWithTram, bool& tramCreated, TramImpl** tramPtr) {
    printLinesAndStops(sipProxy);
    std::cout << "Choose line or type 0 to go back: ";
    int option;

    auto lines = sipProxy->getLines();

    while (1)
    {
        fflush(stdin);
        std::cin >> option;
        if (option == 0) return;

        if ((option - 1) < 0 || ((size_t)option - 1) >= lines.size()) {
            std::cout << "Incorrect input." << std::endl;
            continue;
        }

        break;
    }
    
    auto line = lines.at(option - 1);


    int tramID;
    while (1)
    {
        fflush(stdin);
        std::cout << "Type tram ID or 0 to go back: ";
        std::cin >> tramID;
        if (tramID == 0) return;

        if (tramID < 0) {
            std::cout << "Incorrect input." << std::endl;
            continue;
        }

        break;
    }

    auto stopsOnLine = line->getStops();
    for (size_t i = 0; i < stopsOnLine.size();)
    {
        short int hour;
        short int minute;
        auto stop = stopsOnLine.at(i);
        auto stopID = stop->getID();
        auto stopName = stop->getName();
        std::cout << "Stop ID-NAME: " + std::to_string(stopID) + " - " + stopName << std::endl;
        fflush(stdin);
        std::cout << "Set arrival hour: ";
        std::cin >> hour;
        std::cout << "Set arrival minute: ";
        std::cin >> minute;
        schedule.push_back({stop, {hour, minute}});
        i++;
    }

    auto tram = new TramImpl(tramID);
    auto tramPrx = MPK::TramPrx::checkedCast(adapter->addWithUUID(tram));

    line->addTram(tramPrx);
    tram->setCurrentStop(line->getStops().at(0));
    tram->setSchedule(schedule);

    newTram = tramPrx;
    tramCreated = true;

    lineWithTram = line;

    *tramPtr = tram;

    auto tramSchedule = tramPrx->getSchedule();
    for (size_t i = 0; i < tramSchedule.size(); i++)
    {
        tramSchedule.at(i).stop->updateUsers();
    }
    
}

int main(int argc, char* argv[])
{

    MPK::schedule schedule;
    MPK::TramPrx tram;
    MPK::LinePrx lineWithTram;
    bool tramCreated = false;

    TramImpl* tramPtr;

    try {
        // Inicjalizacja komunikatora Ice
        Ice::CommunicatorHolder ich(argc, argv, "tram/tram.config");
        auto communicator = ich.communicator();


        auto sipProxy = Ice::checkedCast<MPK::SIPPrx>(
        communicator->propertyToProxy("System.Proxy")->ice_twoway()->ice_secure(false));
        if (!sipProxy) {
            throw std::runtime_error("Invalid proxy");
        }

        auto adapter = communicator->createObjectAdapterWithEndpoints("TramAdapter", "default");
        adapter->activate();

        bool run = true;
        bool tramIsRunning = false;
        while (run && !tramIsRunning)
        {
            fflush(stdin);
            std::cout << "1 - Display lines and stops" << std::endl;
            std::cout << "2 - Add tram to line" << std::endl;
            std::cout << "0 - EXIT" << std::endl;

            int action;
            std::cout << "Type action: ";
            std::cin >> action;

            switch (action)
            {
            case 0:
                run = false;
                break;
            case 1:
                printLinesAndStops(sipProxy);
                break;
            case 2:
                addTramToLine(sipProxy, adapter, schedule, tram, lineWithTram, tramCreated, &tramPtr);
                if (tramCreated) {
                    tramIsRunning = true;
                }
                break;
            
            default:
                std::cout << "Unknown action!" << std::endl;
                break;
            }
        }
        
        while (tramIsRunning)
        {
            std::cout << "\nTram current stop: " + tram->getStop()->getName() + "\n" << std::endl;
            int action;
            std::cout << "1 - Move to next stop." << std::endl;
            std::cout << "0 - EXIT" << std::endl;
            std::cin >> action;
            if (action == 0) break;
            else if (action == 1) {
                tramPtr->moveTram(tramIsRunning, tram);
            }else {
                std::cout << "Incorrect input." << std::endl;
            }
        }

        if (tramCreated) {
            lineWithTram->removeTram(tram);
        }

        
        tramPtr->unregisterUsers();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        if (tramCreated) {
            lineWithTram->removeTram(tram);
        }
        return 1;
    }

    return 0;
}
