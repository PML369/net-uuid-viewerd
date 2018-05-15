
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
	typedef std::vector<pid_t> pidlist_t;

	RbTst		<char, PacketInfo>		uuidTrie;
	RbTst		<char, infolist_t>		addrTrie;
	RbTst		<char, pidlist_t>		procNamePidTrie;
	std::multimap	<pid_t, std::string>		pidSocketMap;
	RbTst		<char, infolist_t>		socketTrie;
	std::map	<std::string, PacketInfo *>	uuidCacheMap;

	NetUuidData();
};

#endif /*NET_UUID_DATA_HPP*/
