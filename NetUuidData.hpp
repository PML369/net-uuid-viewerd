
#ifndef NET_UUID_DATA_HPP
#define NET_UUID_DATA_HPP

#include "PacketInfo.hpp"
#include "RbtTrie/RbtTrie.hpp"
#include <sys/types.h>
#include <map>
#include <vector>
#include <string>

class NetUuidData
{
public:
	RbtTrie		<char, PacketInfo>		uuidTrie;
	RbtTrie		<char, std::vector<PacketInfo *> >	addrTrie;
	std::multimap	<std::string, pid_t>		procNameMap;
	std::multimap	<pid_t, PacketInfo *>		pidTrie;
	std::map	<std::string, PacketInfo *>	uuidCacheMap;

	NetUuidData();
};

#endif /*NET_UUID_DATA_HPP*/