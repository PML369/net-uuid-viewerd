#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_layer_arrive::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::layer-arrive
		{
			printf("layer-arrive %s %s %d",
					uuidtostr((uintptr_t)arg0),
					stringof(arg1),
					timestamp);
		});
}

std::string
Probe_layer_arrive::getBufferPrefix(void)
{
	return "layer-arrive";
}

void
Probe_layer_arrive::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, layer, timestamp;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, layer, ' ');
	std::getline(buffer, timestamp, ' ');

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	if (layer == "IP") {
		pkt->timeIpArrive = stoll(timestamp);
		pkt->hasTimeIpArrive = true;
	} else if (layer == "TCP") {
		pkt->timeTcpArrive = stoll(timestamp);
		pkt->hasTimeTcpArrive = true;
	} else if (layer == "UDP") {
		pkt->timeUdpArrive = stoll(timestamp);
		pkt->hasTimeUdpArrive = true;
	}
}
