
#ifndef NET_UUID_DATA_HPP
#define NET_UUID_DATA_HPP

#include "PacketInfo.hpp"
#include "RbTst/RbTst.hpp"
#include <sys/types.h>
#include <map>
#include <vector>
#include <string>

class NetUuidData
{
public:
	typedef std::vector<PacketInfo *> infolist_t;

	RbTst		<char, PacketInfo>		uuidTrie;
	RbTst		<char, infolist_t>		addrTrie;
	std::multimap	<std::string, pid_t>		procNamePidMap;
	std::multimap	<pid_t, std::string>		pidSocketMap;
	RbTst		<char, infolist_t>		socketTrie;
	std::map	<std::string, PacketInfo *>	uuidCacheMap;

	NetUuidData();
};

#endif /*NET_UUID_DATA_HPP*/
