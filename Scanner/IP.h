#ifndef IP_H
#define IP_H

#include<string>

class IP
{
public:
	enum familyType { IPv4, IPv6 };
private:
	familyType family;
	std::string address;
	unsigned int port;

	bool (*checkIP[2])(std::string) = { IP::checkIPv4, IP::checkIPv6 };
public:
	IP();
	IP(std::string);
	IP(const char*);
	explicit IP(familyType, std::string, unsigned int);

	static bool checkIPv4(std::string);
	static bool checkIPv6(std::string);
	std::string getIP()const;
	unsigned int getPort()const;
	familyType getType()const;

	void setIP(std::string );
	void setPort(unsigned int);

};

#endif

