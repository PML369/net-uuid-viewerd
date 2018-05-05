#include "ProbeTypes.hpp"

#include <algorithm>
#include <string>

std::string
Probe_trace_start::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::trace-start
		{
			printf("trace-start %s %d %p",
					uuidtostr((uintptr_t)arg0),
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

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	pkt->timeCreated = stoll(timestamp);
	pkt->hasTimeCreated = true;
	pkt->pushAddress(address);

	getOrCreateAddressVector(address, data)
		->push_back(pkt);
}
