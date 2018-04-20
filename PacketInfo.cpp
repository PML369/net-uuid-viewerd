#include "PacketInfo.hpp"
#include <string>

PacketInfo::PacketInfo()
{
	hasTimeCreated = false;
	hasTimeDeleted = false;
	wasDropped = false;
}
void
PacketInfo::setUuid(std::string &str)
{
	stringToUuid(uuid, str);
}
void
PacketInfo::setSocket(std::string &str)
{
	stringToUuid(socketUuid, str);
}
void
PacketInfo::stringToUuid(char *uuid, std::string &str)
{
	memcpy(uuid, str.c_str(), std::max((size_t)36, str.length()));
}
void
PacketInfo::pushAddress(std::string &str)
{
	void *addr;
	sscanf(str.c_str(), "%p", &addr);
	addresses.push_back(addr);
}
