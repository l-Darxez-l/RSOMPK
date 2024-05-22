#include <Ice/Ice.h>

#include "MPK.h"

class LineImpl : public MPK::Line
{
private:
    int _id;
    std::vector<MPK::TramPrx> _trams; //Lista tramwajów na tej linii
    std::vector<MPK::StopPrx> _stops; //Lista przystanków obsługiwanych przez tę linię
public:
    LineImpl();
    LineImpl(int id);
    ~LineImpl();

    virtual MPK::trams getTrams(const Ice::Current&) override;
    virtual MPK::stops getStops(const Ice::Current&) override;
    virtual void addTram(const MPK::TramPrx& tram, const Ice::Current&) override;
    virtual void removeTram(const MPK::TramPrx& tram, const Ice::Current&) override;

    //Metoda ta nie jest widoczna poza serwerem ICE
    void addStop(const MPK::StopPrx& stop);
};
