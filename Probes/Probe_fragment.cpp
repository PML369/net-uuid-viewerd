#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_fragment::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::fragment
		{
			printf("fragment %s %s", stringof(arg0),stringof(arg1));
		});
}

std::string
Probe_fragment::getBufferPrefix(void)
{
	return "fragment";
}

void
Probe_fragment::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, fragUuid;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, fragUuid, ' ');

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	PacketInfo *fragPkt = getOrCreateEntry(fragUuid, data);

	pkt->fragments.push_back(fragPkt);
	fragPkt->fragmentOf = pkt;
}
