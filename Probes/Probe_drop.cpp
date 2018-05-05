#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_drop::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::drop
		{
			printf("drop %s %d",
					uuidtostr((uintptr_t)arg0),
					timestamp);
		});
}

std::string
Probe_drop::getBufferPrefix(void)
{
	return "drop";
}

void
Probe_drop::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, timestamp;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, timestamp, ' ');

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	pkt->timeDropped = stoll(timestamp);
	pkt->wasDropped = true;
}
