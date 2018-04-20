#include "ProbeType.hpp"
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
	pkt->setUuid(uuid);
	data->uuidTrie.insert(uuid.c_str(), uuid.length(), pkt);
	data->uuidCacheMap.emplace(uuid, pkt);
	return pkt;
}

NetUuidData::infolist_t *
ProbeType::getOrCreateAddressVector(std::string &address, NetUuidData *data)
{
	return getOrCreatePacketInfoVector(address, &data->addrTrie);
}
NetUuidData::infolist_t *
ProbeType::getOrCreateSocketVector(std::string &socket, NetUuidData *data)
{
	return getOrCreatePacketInfoVector(socket, &data->socketTrie);
}
NetUuidData::infolist_t *
ProbeType::getOrCreatePacketInfoVector(std::string &key,
			RbtTrie<char, NetUuidData::infolist_t> *trie)
{
	NetUuidData::infolist_t *vec = trie->get(key.c_str(), key.length());
	if (vec != NULL)
		return vec;

	vec = new NetUuidData::infolist_t();
	trie->insert(key.c_str(), key.length(), vec);
	return vec;
}
