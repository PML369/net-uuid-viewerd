#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_to_socket::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::to-socket
		{
			printf("to-socket %s %s",
					uuidtostr((uintptr_t)arg0),
					uuidtostr((uintptr_t)arg1));
		});
}

std::string
Probe_to_socket::getBufferPrefix(void)
{
	return "to-socket";
}

void
Probe_to_socket::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string pktUuid, socketUuid;
	std::getline(buffer, pktUuid, ' ');
	std::getline(buffer, socketUuid, ' ');

	PacketInfo *pkt = getOrCreateEntry(pktUuid, data);
	pkt->setSocket(socketUuid);
	pkt->hasSocketTo = true;
	getOrCreateSocketVector(socketUuid, data)
		->push_back(pkt);
}
