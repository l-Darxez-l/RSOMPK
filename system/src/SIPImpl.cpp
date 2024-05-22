#include "SIPImpl.h"
#include <iostream>


SIPImpl::SIPImpl() {
    
}


SIPImpl::~SIPImpl() {
    
}

MPK::lines SIPImpl::getLines(const Ice::Current& current) {
    return _lines;
}

void SIPImpl::addLine(const MPK::LinePrx& line, const Ice::Current& current) {

    for (auto &existingLine : _lines)
    {
        if (existingLine == line) {
            std::cerr << "Line already exists!" << std::endl;
            return;
        }
    }

    _lines.push_back(line);
    std::cout << "Line added successfully." << std::endl;
}

void SIPImpl::removeLine(const MPK::LinePrx& line, const Ice::Current& current) {
    auto it = std::find(_lines.begin(), _lines.end(), line);
    if (it != _lines.end()) {
        _lines.erase(it);
        std::cout << "Line removed successfully." << std::endl;
    }else {
        std::cerr << "Line not found!" << std::endl;
        
        throw MPK::noSuchLine{};
    }
}

MPK::StopPrx SIPImpl::getStop(int ID, const Ice::Current& current) {

    auto it = _stops.find(ID);
    if (it != _stops.end()) {
        return it->second;
    }else {
        std::cerr << "Stop not found!" << std::endl;
        throw MPK::noSuchStop{};
    }
}

void SIPImpl::addStop(const MPK::StopPrx& stop) {
    _stops[stop->getID()] = stop;
}