#include "UserImpl.h"
#include <iostream>

UserImpl::UserImpl() {

}

UserImpl::UserImpl(const std::string& username) : _username(username) {

}

UserImpl::~UserImpl() {

}

void UserImpl::updateStop(const MPK::TramPrx& tram, const MPK::StopPrx& stop, const Ice::Current& current) {
    std::cout << "\n__________Update received for user " << _username << "__________" << std::endl;
    std::cout << "\n" << "Tram ID " << tram->getID() << " is currently at Stop: " << stop->getName() << std::endl;
}

void UserImpl::updateSchedule(int stopID, const std::string& stopName, const MPK::arrivals& arr, const Ice::Current& current) {
    std::cout << "\n__________Update received for user " << _username << "__________" << std::endl;
    std::cout << "\nSchedule update for Stop ID - NAME: " << std::to_string(stopID) + " - " + stopName << std::endl;
    for (const auto& item : arr) {
        std::cout << "Tram ID: " << item.tram->getID()
            << " at " << item.time.hour << ":" << item.time.minute << std::endl;
    }
}