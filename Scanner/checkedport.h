#ifndef CHECKEDPORT_H
#define CHECKEDPORT_H

#include <stdexcept>


class CheckedPort
{
public:
    enum protocolType {TCP, UDP};
    bool result;
    protocolType type;
    unsigned int port;
    CheckedPort();
    explicit CheckedPort(bool res, protocolType t, unsigned int port);

    bool operator< (const CheckedPort& sec)const;
};

#endif // CHECKEDPORT_H
