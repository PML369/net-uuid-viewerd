#include "ProbeTypes.hpp"

#include <string>

std::string
Probe_to_subsys::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:packet::to-subsys
		{
			printf("to-subsys %s %s", stringof(arg0),
							stringof(arg1));
		});
}

std::string
Probe_to_subsys::getBufferPrefix(void)
{
	return "to-subsys";
}

void
Probe_to_subsys::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, subsys;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, subsys, ' ');

	getOrCreateEntry(uuid, data)
		->subsystemsTouched.push_back(subsys);
}
