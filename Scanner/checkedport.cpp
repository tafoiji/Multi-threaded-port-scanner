#include "checkedport.h"

CheckedPort::CheckedPort(): result(false), type(SOCK_STREAM), port(0) {}

CheckedPort::CheckedPort(bool res, DWORD t, unsigned int port)    : result(res), type(t), port(port)
{
    if (t != SOCK_STREAM && t != SOCK_DGRAM)
    {
        throw std::invalid_argument("invalid type of protocol");
    }
}

bool CheckedPort::operator< (const CheckedPort& sec)const
{
    if (this->result != sec.result)
    {
        return this->result > sec.result;
    }

    if (this->port != sec.port)
    {
        return this->port < sec.port;
    }

    return this->type < sec.type;
}
