#include <Ice/Ice.h>
#include "UserImpl.h"
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

void printTramsOnLine(const MPK::SIPPrx& sipProxy) {
    fflush(stdin);

    int lineNumber;
    std::cout << "Type line number: ";
    std::cin >> lineNumber;

    auto lines = sipProxy->getLines();

    if ((lineNumber - 1) < 0 || ((size_t)lineNumber - 1) >= lines.size()) {
        std::cout << "No such line." << std::endl;
        return;
    }

    auto line = lines.at(lineNumber - 1);

    auto tramsOnLine = line->getTrams();
    if (tramsOnLine.empty()) {
        std::cout << "There are currently no trams on this line." << std::endl;
        return;
    }

    for (size_t i = 0; i < tramsOnLine.size(); i++)
    {
        auto tramID = tramsOnLine.at(i)->getID();
        auto tramSchedule = tramsOnLine.at(i)->getSchedule();
        std::cout << "__________Timetable for Tram with ID: " + std::to_string(tramID) << std::endl;
        for (size_t j = 0; j < tramSchedule.size(); j++)
        {
            auto stop = tramSchedule.at(j).stop;
            auto stopTime = tramSchedule.at(j).time;
            std::cout << "Stop ID - Stop Name - Stop Time: " + std::to_string(stop->getID()) + " - " + stop->getName() 
                + " - " + std::to_string(stopTime.hour) + ":" + std::to_string(stopTime.minute) << std::endl;

        }

        std::cout << "____________________" << std::endl;
        
    }
    
}

void printStopSchedule(const MPK::SIPPrx& sipProxy) {
    fflush(stdin);

    int stopID;
    std::cout << "Type Stop ID: ";
    std::cin >> stopID;

    try
    {
        auto stop = sipProxy->getStop(stopID);
        auto stopArrivals = stop->getArrivals();
        if (stopArrivals.empty()) {
            std::cout << "No arrivals." << std::endl;
            return;
        }

        for (size_t i = 0; i < stopArrivals.size(); i++)
        {
            auto tramID = stopArrivals.at(i).tram->getID();
            auto arrivalTime = stopArrivals.at(i).time;

            std::cout << "Tram ID: " + std::to_string(tramID) + " | Arrival time: "
                + std::to_string(arrivalTime.hour) + ":" + std::to_string(arrivalTime.minute) << std::endl;
        }
        

    }
    catch(const MPK::noSuchStop& e)
    {
        std::cerr << "No stop with the given ID" << '\n';
    }
    
}

void registerUserToTram(const MPK::SIPPrx& sipProxy, const MPK::UserPrx& userProxy, std::vector<MPK::TramPrx>& userTrams) {
    fflush(stdin);

    int lineNumber;
    std::cout << "Type line number: ";
    std::cin >> lineNumber;

    auto lines = sipProxy->getLines();

    if ((lineNumber - 1) < 0 || ((size_t)lineNumber - 1) >= lines.size()) {
        std::cout << "No such line." << std::endl;
        return;
    }

    auto line = lines.at(lineNumber - 1);

    auto tramsOnLine = line->getTrams();
    if (tramsOnLine.empty()) {
        std::cout << "There are currently no trams on this line." << std::endl;
        return;
    }

    for (size_t i = 0; i < tramsOnLine.size(); i++)
    {
        auto tramID = tramsOnLine.at(i)->getID();
        auto tramSchedule = tramsOnLine.at(i)->getSchedule();
        std::cout << "__________Timetable for Tram with ID: " + std::to_string(tramID) << std::endl;
        for (size_t j = 0; j < tramSchedule.size(); j++)
        {
            auto stop = tramSchedule.at(j).stop;
            auto stopTime = tramSchedule.at(j).time;
            std::cout << "Stop ID - Stop Name - Stop Time: " + std::to_string(stop->getID()) + " - " + stop->getName() 
                + " - " + std::to_string(stopTime.hour) + ":" + std::to_string(stopTime.minute) << std::endl;

        }

        std::cout << "____________________" << std::endl;
        
    }

    fflush(stdin);

    int tramID;
    std::cout << "Type Tram ID: ";
    std::cin >> tramID;

    MPK::TramPrx tramToRegisterUser;

    bool tramFound = false;
    for (size_t i = 0; i < tramsOnLine.size(); i++)
    {
        if (tramsOnLine.at(i)->getID() == tramID) {
            tramToRegisterUser = tramsOnLine.at(i);
            tramFound = true;
            break;
        }
    }

    if (!tramFound) {
        std::cout << "Incorrect Tram ID." << std::endl;
        return;
    }

    tramToRegisterUser->registerUser(userProxy);

}

void registerUserToStop(const MPK::SIPPrx& sipProxy, const MPK::UserPrx& userProxy, std::vector<MPK::StopPrx>& userStops) {

    try
    {
        printLinesAndStops(sipProxy);

        int stopId;
        std::cout << "Type Stop ID: ";
        std::cin >> stopId;
    

        auto stop = sipProxy->getStop(stopId);
        stop->registerUser(userProxy);
        userStops.push_back(stop);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char* argv[])
{
    try {
        // Inicjalizacja komunikatora Ice
        Ice::CommunicatorHolder ich(argc, argv, "client/user.config");
        auto communicator = ich.communicator();

        // Tworzenie proxy dla interfejsu SIP
        auto sipProxy = Ice::checkedCast<MPK::SIPPrx>(
            communicator->propertyToProxy("System.Proxy")->ice_twoway()->ice_secure(false));
        if (!sipProxy) {
            throw std::runtime_error("Invalid proxy");
        }


        auto adapter = communicator->createObjectAdapterWithEndpoints("ClientAdapter", "default");
        adapter->activate();

        std::string userName;
        std::vector<MPK::TramPrx> userTrams;
        std::vector<MPK::StopPrx> userStops;

        std::cout << "Type your username: ";
        std::cin >> userName;

        auto client = new UserImpl(userName);
        auto clientPrx = MPK::UserPrx::checkedCast(adapter->addWithUUID(client));

        bool end = false;

        while (!end)
        {
            fflush(stdin);
            std::cout << "1 - Display lines and stops" << std::endl;
            std::cout << "2 - Display the trams that are currently on a given tram line." << std::endl;
            std::cout << "3 - Display the arrival schedule for a given stop." << std::endl;
            std::cout << "4 - Register a user to a tram of a given id." << std::endl;
            std::cout << "5 - Register a user to a stop of a given id." << std::endl;
            std::cout << "0 - EXIT" << std::endl;

            int action;
            std::cout << "Type action: ";
            std::cin >> action;

            switch (action)
            {
            case 0:
                end = true;
                break;
            case 1:
                printLinesAndStops(sipProxy);
                break;
            case 2:
                printTramsOnLine(sipProxy);
                break;
            case 3:
                printStopSchedule(sipProxy);
                break;
            case 4:
                registerUserToTram(sipProxy, clientPrx, userTrams);
                break;
            case 5:
                registerUserToStop(sipProxy, clientPrx, userStops);
                break;
            
            default:
                std::cout << "Unknown action!" << std::endl;
                break;
            }
        }
        
        for (size_t i = 0; i < userTrams.size(); i++)
        {
            userTrams.at(i)->unregisterUser(clientPrx);
        }

        for (size_t i = 0; i < userStops.size(); i++)
        {
            userStops.at(i)->unregisterUser(clientPrx);
        }
        
        
        
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
