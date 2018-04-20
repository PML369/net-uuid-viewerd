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

	PacketInfo *pkt = getOrCreateEntry(uuid, data);
	pkt->timeCreated = stoll(timestamp);
	pkt->hasTimeCreated = true;
	pkt->pushAddress(address);

	std::vector<PacketInfo *> *vec = getOrCreateAddressVector(address,
									data);
	vec->push_back(pkt);

	std::cout << "id=" << uuid << " t=" << timestamp << " addr=" << address << std::endl;
}
