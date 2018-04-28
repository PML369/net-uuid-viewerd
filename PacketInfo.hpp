
#ifndef PACKET_INFO_HPP
#define PACKET_INFO_HPP

#include <vector>

class PacketInfo
{
public:
	typedef uint64_t timestamp_t;

	char uuid[36];
	char socketUuid[36];
	std::vector<void *> addresses;
	std::vector<std::string> subsystemsTouched;
	std::vector<PacketInfo *> fragments;
	PacketInfo *fragmentOf;

	timestamp_t timeCreated;
	timestamp_t timeDeleted;
	timestamp_t timeDropped;
	timestamp_t timeIpArrive;
	timestamp_t timeIpDepart;
	timestamp_t timeTcpArrive;
	timestamp_t timeTcpDepart;
	timestamp_t timeUdpArrive;
	timestamp_t timeUdpDepart;

	bool hasTimeCreated : 1;
	bool hasTimeDeleted : 1;
	bool wasDropped	    : 1;
	bool hasTimeIpArrive : 1;
	bool hasTimeIpDepart : 1;
	bool hasTimeTcpArrive : 1;
	bool hasTimeTcpDepart : 1;
	bool hasTimeUdpArrive : 1;
	bool hasTimeUdpDepart : 1;
	bool hasSocketFrom  : 1;
	bool hasSocketTo    : 1;

	PacketInfo();
	void setUuid(std::string &);
	void setSocket(std::string &);
	void pushAddress(std::string &);
private:
	void stringToUuid(char *, std::string &);
};

#endif /*PACKET_INFO_HPP*/
