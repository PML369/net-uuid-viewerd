#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_trace_stop::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::trace-stop
		{
			printf("trace-stop %s %d", stringof(arg0), timestamp);
		});
}

std::string
Probe_trace_stop::getBufferPrefix(void)
{
	return "trace-stop";
}

void
Probe_trace_stop::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, timestamp;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, timestamp, ' ');

	PacketInfo *pkt;

	auto it = data->uuidCacheMap.find(uuid);
	if (it != data->uuidCacheMap.end())
	{
		pkt = it->second;
		data->uuidCacheMap.erase(it);
	}
	else
	{
		pkt = data->uuidTrie.get(uuid.c_str(), uuid.length());
	}
	if (pkt == NULL)
		return;

	pkt->timeDeleted = stoll(timestamp);
	pkt->hasTimeDeleted = true;
}
