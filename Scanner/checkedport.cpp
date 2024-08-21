#include "checkedport.h"

CheckedPort::CheckedPort(): result(false), type(TCP), port(0) {}

CheckedPort::CheckedPort(bool res, protocolType t, unsigned int port)    : result(res), port(port)
{
    if (t != TCP && t != UDP)
    {
        throw std::invalid_argument("invalid type of protocol");
    }

    type = t;
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
