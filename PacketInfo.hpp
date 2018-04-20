
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

	bool hasTimeCreated : 1;
	bool hasTimeDeleted : 1;
	bool wasDropped	    : 1;
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
