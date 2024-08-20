#ifndef CHECKEDPORT_H
#define CHECKEDPORT_H

#include <Windows.h>
#include <stdexcept>


class CheckedPort
{
public:
    bool result;
    DWORD type;
    unsigned int port;
    CheckedPort();
    CheckedPort(bool res, DWORD t, unsigned int port);

    bool operator< (const CheckedPort& sec)const;
};

#endif // CHECKEDPORT_H
