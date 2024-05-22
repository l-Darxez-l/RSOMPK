#include <Ice/Ice.h>
#include "MPK.h"


class UserImpl : public MPK::User
{
private:
    std::string _username;
public:
    UserImpl();
    UserImpl(const std::string& username);
    ~UserImpl();

    virtual void updateStop(const MPK::TramPrx& tram, const MPK::StopPrx& stop, const Ice::Current&) override;
    virtual void updateSchedule(int stopID, const std::string& stopName, const MPK::arrivals& arr, const Ice::Current&) override;
};

