#include <Ice/Ice.h>
#include "MPK.h"


class TramImpl : public MPK::Tram
{
private:
    int _id;
    MPK::StopPrx _currentStop;
    MPK::schedule _schedule;
    std::vector<MPK::UserPrx> _registeredUsers;
public:
    TramImpl();
    TramImpl(int id);
    ~TramImpl();


    virtual MPK::StopPrx getStop(const Ice::Current&) override;
    virtual MPK::TimeOfDay getStopTime(int stopID, const Ice::Current&) override;
    virtual int getID(const Ice::Current&) override;
    virtual MPK::schedule getSchedule(const Ice::Current&) override;
    virtual void registerUser(const MPK::UserPrx& user, const Ice::Current&) override;
    virtual void unregisterUser(const MPK::UserPrx& user, const Ice::Current&) override;

    void setCurrentStop(const MPK::StopPrx& stop);
    void setSchedule(const MPK::schedule& schedule);
    void moveTram(bool& tramIsRunning, const MPK::TramPrx& tramProxy);

    void unregisterUsers();
};
