#include "ProbeTypes.hpp"

#include <iostream>

std::string
Probe_trace_start::getDScript()
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
Probe_trace_start::getBufferPrefix()
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

	std::cout << "id=" << uuid << " t=" << timestamp << " addr=" << address << std::endl;
}
