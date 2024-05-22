#include <Ice/Ice.h>

#include "MPK.h"

class SIPImpl : public MPK::SIP
{
private:
    std::map<int, MPK::StopPrx> _stops;  // Przechowuje proxy do przystanków
    std::vector<MPK::LinePrx> _lines;    // Lista proxy do linii


public:
    SIPImpl();
    ~SIPImpl();

    virtual MPK::lines getLines(const Ice::Current&) override;
    virtual void addLine(const MPK::LinePrx& line, const Ice::Current&) override;
    virtual void removeLine(const MPK::LinePrx& line, const Ice::Current&) override;
    virtual MPK::StopPrx getStop(int ID, const Ice::Current&) override;


    void addStop(const MPK::StopPrx& stop);
};
