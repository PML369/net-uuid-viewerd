#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_from_socket::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::from-socket
		{
			printf("from-socket %s %s",
					uuidtostr((uintptr_t)arg0),
					uuidtostr((uintptr_t)arg1));
		});
}

std::string
Probe_from_socket::getBufferPrefix(void)
{
	return "from-socket";
}

void
Probe_from_socket::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string pktUuid, socketUuid;
	std::getline(buffer, pktUuid, ' ');
	std::getline(buffer, socketUuid, ' ');

	PacketInfo *pkt = getOrCreateEntry(pktUuid, data);
	pkt->setSocket(socketUuid);
	pkt->hasSocketFrom = true;
	getOrCreateSocketVector(socketUuid, data)
		->push_back(pkt);
}
