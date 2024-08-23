#include "IP.h"
#include <stdexcept>
#include <map>

IP::IP() {}

IP::IP(std::string ip): port(0)
{
    if (checkIPv4(ip))
    {
        this->family = IPv4;
        this->address = ip;
    }
    else if (checkIPv6(ip))
    {
        this->family = IPv6;
        this->address = ip;
    }
    else
    {
        throw std::invalid_argument("ip address is invalid");
    }
}

IP::IP(const char* ip): IP(std::string(ip)) {}

IP::IP(familyType type, std::string addr, unsigned int port)
{
    if (port >= (1 << 16))
    {
        throw std::invalid_argument("port value is invalid");
    }

    if (!checkIP[type](addr))
    {
        throw std::invalid_argument("ip address is invalid");
    }

    this->family = type;
    this->address = addr;
    this->port = port;
}

bool IP::checkIPv4(std::string addr)
{
    int cntPoints = 0;
    if (addr.size() < 2 || *addr.begin() == '.' || *addr.rbegin() == '.')
    {
        return false;
    }

    for (auto& i : addr)
    {
        cntPoints += (i == '.');
    }

    if (cntPoints != 3)
    {
        return false;
    }

    for (int i = 1; i < (int)addr.size() - 1; i++)
    {
        if ((addr[i] == addr[i - 1] && addr[i] == '.') || (addr[i - 1] == '.' && addr[i] == '0' && addr[i + 1] != '.'))
        {
            return false;
        }
    }

    int value = 0;
    for (auto& i : addr)
    {
        if (i == '.' && value >= 0x00 && value <= 0xff)
        {
            value = 0;
        }
        else if (i >= '0' && i <= '9')
        {
            value = value * 10 + i - '0';
        }
        else
        {
            return false;
        }

        if (value > 0xff)
        {
            return false;
        }
    }

    return true;
}

bool IP::checkIPv6(std::string addr)
{
    if (addr.size() < 2)
    {
        return false;
    }

    const std::map<char, int> translateFromHex  = { {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15} };

    for (auto& i : addr)
    {
        i = toupper(i);
    }

    int cntColons = 0;
    int cntPoints = 0;
    for (auto& i : addr)
    {
        cntColons += (i == ':');
        cntPoints += (i == '.');
    }

    int cntSeq = 0;
    for (int i = 1; i < (int)addr.size(); i++)
    {
        cntSeq += (addr[i] == addr[i - 1] && addr[i] == ':');
    }

    if (cntSeq > 1)
    {
        return false;
    }

    if (cntSeq == 1 && cntColons > 7)
    {
        return false;
    }

    if (cntSeq == 0 && cntColons != 7)
    {
        return false;
    }

    if (cntPoints != 0 && cntPoints != 3)
    {
        return false;
    }
    
    if (cntPoints == 0)
    {
        int value = 0;
        for (auto& i : addr)
        {
            if (i == ':' && value >= 0x00 && value <= 0xffff)
            {
                value = 0;
            }
            else if ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'F'))
            {
                value = value * 16 + translateFromHex.at(i);
            }
            else
            {
                return false;
            }

            if (value > 0xffff)
            {
                return false;
            }
        }
    }
    else
    {
        std::string ipv4 = "";
        std::string ipv6 = "";
        int indColon;
        for (indColon = addr.size() - 1; indColon >= 0; indColon--)
        {
            if (addr[indColon] == ':')
            {
                break;
            }
        }

        for (int i = 0; i < (int)addr.size(); i++)
        {
            if (i < indColon)
            {
                ipv6 += addr[i];
            }
            else if (i > indColon)
            {
                ipv4 += addr[i];
            }
        }

        int value = 0;
        for (auto& i : ipv6)
        {
            if (i == ':' && value >= 0x00 && value <= 0xffff)
            {
                value = 0;
            }
            else if ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'F'))
            {
                value = value * 16 + translateFromHex.at(i);
            }
            else
            {
                return false;
            }

            if (value > 0xffff)
            {
                return false;
            }
        }

        if (!checkIPv4(ipv4))
        {
            return false;
        }
    }

    return true;
}

std::string IP::getIP() const
{
    return this->address;
}

unsigned int IP::getPort() const
{
    return this->port;
}

IP::familyType IP::getType() const
{
    return this->family;
}

void IP::setIP(std::string addr)
{
    if (!checkIP[this->family](addr))
    {
        throw std::invalid_argument("ip address is invalid");
    }
    
    this->address = addr;
}

void IP::setPort(unsigned int port)
{
    if (port >= (1 << 16))
    {
        throw std::invalid_argument("port value is invalid");
    }

    this->port = port;
}
