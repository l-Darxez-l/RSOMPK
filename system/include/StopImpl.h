#include <Ice/Ice.h>

#include "MPK.h"


class StopImpl : public MPK::Stop
{
private:
    int _id;
    std::string _name;
    std::vector<MPK::LinePrx> _lines; // Linie przechodzące przez ten przystanek
    MPK::arrivals _arrivals; // Informacje o przyjazdach
    std::vector<MPK::UserPrx> _registeredUsers; // Użytkownicy zarejestrowani do powiadomień


    MPK::arrivals createArrivals();
public:
    StopImpl();
    StopImpl(int id, const std::string& name);
    ~StopImpl();


    virtual MPK::lines getLines(const Ice::Current&) override;
    virtual int getID(const Ice::Current&) override;
    virtual std::string getName(const Ice::Current&) override;
    virtual MPK::arrivals getArrivals(const Ice::Current&) override;
    virtual void registerUser(const MPK::UserPrx& user, const Ice::Current&) override;
    virtual void unregisterUser(const MPK::UserPrx& user, const Ice::Current&) override;
    virtual void updateUsers(const Ice::Current&) override;

    void addLine(const MPK::LinePrx& line);
};
