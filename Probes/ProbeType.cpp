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
	return getOrCreateVector(address, &data->addrTrie);
}
NetUuidData::infolist_t *
ProbeType::getOrCreateSocketVector(std::string &socket, NetUuidData *data)
{
	return getOrCreateVector(socket, &data->socketTrie);
}
NetUuidData::pidlist_t *
ProbeType::getOrCreateProcPidVector(std::string &procName, NetUuidData *data)
{
	return getOrCreateVector(procName, &data->procNamePidTrie);
}
template <typename T>
T *
ProbeType::getOrCreateVector(std::string &key, RbTst<char, T> *trie)
{
	T *vec = trie->get(key.c_str(), key.length());
	if (vec != NULL)
		return vec;

	vec = new T();
	trie->insert(key.c_str(), key.length(), vec);
	return vec;
}
