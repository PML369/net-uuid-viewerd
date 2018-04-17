#include "ProbeTypes.hpp"
#include <NetUuidData.hpp>

#include <iostream>
#include <algorithm>
#include <string>

std::string
Probe_trace_start::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::trace-start
		{
			printf("trace-start %s %d %p",
					stringof(arg0),
					timestamp,
					arg1);
		});
}

std::string
Probe_trace_start::getBufferPrefix(void)
{
	return "trace-start";
}

void
Probe_trace_start::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, timestamp, address;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, timestamp, ' ');
	std::getline(buffer, address, ' ');

	PacketInfo *pkt = new PacketInfo();

	memcpy(pkt->uuid, uuid.c_str(), std::max((size_t)36, uuid.length()));
	pkt->timeCreated = stoll(timestamp);
	pkt->hasTimeCreated = true;

	void *addr;
	sscanf(address.c_str(), "%p", &addr);
	pkt->addresses.push_back(addr);

	data->uuidTrie.insert(uuid.c_str(), uuid.length(), pkt);
	data->addrTrie.insert(address.c_str(), address.length(), pkt);
	data->uuidCacheMap.emplace(uuid, pkt);

	std::cout << "id=" << uuid << " t=" << timestamp << " addr=" << address << std::endl;
}
