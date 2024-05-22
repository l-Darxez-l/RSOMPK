#include "StopImpl.h"
#include <algorithm>
#include <iostream>

StopImpl::StopImpl() {
    
}

StopImpl::StopImpl(int id, const std::string& name) : _id(id), _name(name) {

}

StopImpl::~StopImpl() {

}

MPK::lines StopImpl::getLines(const Ice::Current& current) {
    return _lines;
}

int StopImpl::getID(const Ice::Current& current) {
    return _id;
}

std::string StopImpl::getName(const Ice::Current& current) {
    return _name;
}

MPK::arrivals StopImpl::getArrivals(const Ice::Current& current) {
    
    _arrivals = createArrivals();

    return _arrivals;
}

void StopImpl::registerUser(const MPK::UserPrx& user, const Ice::Current& current) {
    if (std::find(_registeredUsers.begin(), _registeredUsers.end(), user) != _registeredUsers.end()) {
        std::cerr << "User is already registered." << std::endl;
        return;
    }
    _registeredUsers.push_back(user);
    std::cout << "User registered successfully." << std::endl;
}

void StopImpl::unregisterUser(const MPK::UserPrx& user, const Ice::Current& current) {
    auto it = std::find(_registeredUsers.begin(), _registeredUsers.end(), user);
    if (it != _registeredUsers.end()) {
        _registeredUsers.erase(it);
        std::cout << "User unregistered successfully." << std::endl;
    } else {
        std::cerr << "User not found." << std::endl;
        throw MPK::noSuchUser{};
    }
}

MPK::arrivals StopImpl::createArrivals() {
    MPK::arrivals arr;
    for (size_t i = 0; i < _lines.size(); i++)
    {
        auto line = _lines.at(i);
        auto tramsOnLine = line->getTrams();

        for (size_t j = 0; j < tramsOnLine.size(); j++)
        {
            auto tram = tramsOnLine.at(j);
            auto stopTime = tram->getStopTime(_id);
            arr.push_back({tram, stopTime});
        }
    }

    return arr;
}

void StopImpl::updateUsers(const Ice::Current& current) {

    MPK::arrivals arr = createArrivals();

    for (size_t i = 0; i < _registeredUsers.size(); i++)
    {
        _registeredUsers.at(i)->updateSchedule(_id, _name, arr);
    }
    
}


void StopImpl::addLine(const MPK::LinePrx& line) {
    _lines.push_back(line);
}