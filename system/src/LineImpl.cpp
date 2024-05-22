#include "LineImpl.h"
#include <algorithm>
#include <iostream>

LineImpl::LineImpl() {
    
}

LineImpl::LineImpl(int id) : _id(id) {

}

LineImpl::~LineImpl() {
    
}

MPK::trams LineImpl::getTrams(const Ice::Current& current) {
    return _trams;
}

MPK::stops LineImpl::getStops(const Ice::Current& current) {
    return _stops;
}

void LineImpl::addTram(const MPK::TramPrx& tram, const Ice::Current& current) {
    if (std::find(_trams.begin(), _trams.end(), tram) != _trams.end()) {
        std::cerr << "Tram already exists on this line." << std::endl;
        return;
    }
    _trams.push_back(tram);
    std::cout << "Tram added successfully to the line." << std::endl;
}

void LineImpl::removeTram(const MPK::TramPrx& tram, const Ice::Current& current) {
    auto it = std::find(_trams.begin(), _trams.end(), tram);
    if (it != _trams.end()) {
        _trams.erase(it);
        std::cout << "Tram removed successfully from the line." << std::endl;
    } else {
        std::cerr << "Tram not found on this line." << std::endl;
        throw MPK::noSuchTram{};
    }
}

void LineImpl::addStop(const MPK::StopPrx& stop) {
    _stops.push_back(stop);
}