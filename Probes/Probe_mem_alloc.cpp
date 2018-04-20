#include "ProbeTypes.hpp"
#include <NetUuidData.hpp>

#include <string>

std::string
Probe_mem_alloc::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:mem::alloc
		{
			printf("alloc %s %p", stringof(arg0), arg1);
		});
}

std::string
Probe_mem_alloc::getBufferPrefix(void)
{
	return "alloc";
}

void
Probe_mem_alloc::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, address;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, address, ' ');

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	pkt->pushAddress(address);

	getOrCreateAddressVector(address,data)
		->push_back(pkt);
}
