#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_layer_depart::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::layer-depart
		{
			printf("layer-depart %s %s %d", stringof(arg0),
							stringof(arg1),
							timestamp);
		});
}

std::string
Probe_layer_depart::getBufferPrefix(void)
{
	return "layer-depart";
}

void
Probe_layer_depart::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, layer, timestamp;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, layer, ' ');
	std::getline(buffer, timestamp, ' ');

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	if (layer == "IP") {
		pkt->timeIpDepart = stoll(timestamp);
		pkt->hasTimeIpDepart = true;
	} else if (layer == "TCP") {
		pkt->timeTcpDepart = stoll(timestamp);
		pkt->hasTimeTcpDepart = true;
	} else if (layer == "UDP") {
		pkt->timeUdpDepart = stoll(timestamp);
		pkt->hasTimeUdpDepart = true;
	}
}
