#include "ProbeType.hpp"
#include "NetUuidData.hpp"
#include "PacketInfo.hpp"

PacketInfo *
ProbeType::getOrCreateEntry(std::string &uuid, NetUuidData *data)
{
	PacketInfo *pkt;

	auto it = data->uuidCacheMap.find(uuid);
	if (it != data->uuidCacheMap.end())
		pkt = it->second;
	else
	{
		pkt = data->uuidTrie.get(uuid.c_str(), uuid.length());
		if (pkt != NULL)
			data->uuidCacheMap.emplace(uuid, pkt);
	}

	if (pkt != NULL)
		return pkt;

	pkt = new PacketInfo();
	data->uuidTrie.insert(uuid.c_str(), uuid.length(), pkt);
	data->uuidCacheMap.emplace(uuid, pkt);
	return pkt;
}

std::vector<PacketInfo *> *
ProbeType::getOrCreateAddressVector(std::string &address, NetUuidData *data)
{
	std::vector<PacketInfo *> *vec = data->addrTrie.get(address.c_str(),
							    address.length());
	if (vec != NULL)
		return vec;

	vec = new std::vector<PacketInfo *>();
	data->addrTrie.insert(address.c_str(), address.length(), vec);
	return vec;
}
