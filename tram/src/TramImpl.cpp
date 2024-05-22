#include "TramImpl.h"
#include <algorithm>
#include <iostream>


TramImpl::TramImpl() {

}

TramImpl::TramImpl(int id) : _id(id) {

}

TramImpl::~TramImpl() {

}

MPK::StopPrx TramImpl::getStop(const Ice::Current& current) {
    return _currentStop;
}

MPK::TimeOfDay TramImpl::getStopTime(int stopID, const Ice::Current& current) {
    for(const auto& item : _schedule) {
        if (item.stop->getID() == stopID) {
            return item.time;
        }
    }

    throw MPK::noSuchStop{};
}

int TramImpl::getID(const Ice::Current& current) {
    return _id;
}

MPK::schedule TramImpl::getSchedule(const Ice::Current& current) {
    return _schedule;
}

void TramImpl::registerUser(const MPK::UserPrx& user, const Ice::Current& current) {
    if (std::find(_registeredUsers.begin(), _registeredUsers.end(), user) != _registeredUsers.end()) {
        std::cerr << "User is already registered." << std::endl;
        return;
    }
    _registeredUsers.push_back(user);
    std::cout << "User registered successfully." << std::endl;
}

void TramImpl::unregisterUser(const MPK::UserPrx& user, const Ice::Current& current) {
    auto it = std::find(_registeredUsers.begin(), _registeredUsers.end(), user);
    if (it != _registeredUsers.end()) {
        _registeredUsers.erase(it);
        std::cout << "User unregistered successfully." << std::endl;
    } else {
        std::cerr << "User not found." << std::endl;
        throw MPK::noSuchUser{};
    }
}

void TramImpl::setCurrentStop(const MPK::StopPrx& stop) {
    _currentStop = stop;
}

void TramImpl::setSchedule(const MPK::schedule& schedule) {
    _schedule = schedule;
}

void TramImpl::moveTram(bool& tramIsRunning, const MPK::TramPrx& tramProxy) {

    int stopIndeks = -1;

    for (size_t i = 0; i < _schedule.size(); i++)
    {
        if (_schedule.at(i).stop == _currentStop) {
            stopIndeks = i + 1;
            break;
        }
    }

    if (stopIndeks == -1 || (size_t)stopIndeks >= _schedule.size()) {
        tramIsRunning = false;
        return;
    }
    
    
    _currentStop = _schedule.at(stopIndeks).stop;

    for (size_t i = 0; i < _registeredUsers.size(); i++)
    {
        _registeredUsers.at(i)->updateStop(tramProxy, tramProxy->getStop());
    }
    
}

void TramImpl::unregisterUsers() {
    _registeredUsers.clear();
    
}