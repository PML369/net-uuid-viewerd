
#ifndef PACKET_INFO_HPP
#define PACKET_INFO_HPP

#include <vector>

class PacketInfo
{
public:
	typedef uint64_t timestamp_t;

	char uuid[36];
	std::vector<void *> addresses;

	timestamp_t timeCreated;
	timestamp_t timeDeleted;
	timestamp_t timeDropped;
	char socketUuid[36];

	bool hasTimeCreated : 1;
	bool hasTimeDeleted : 1;
	bool wasDropped	    : 1;
	bool hasSocketFrom  : 1;
	bool hasSocketTo    : 1;

	PacketInfo();
};

#endif /*PACKET_INFO_HPP*/
