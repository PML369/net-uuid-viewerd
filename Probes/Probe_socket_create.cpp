#include "ProbeTypes.hpp"

#include <string>
#include <iostream>

std::string
Probe_socket_create::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:socket::create
		{
			printf("socket-create %s %d %s\0",
					stringof(args[0]),
					args[1].pr_pid,
					args[1].pr_psargs);
		});
}

std::string
Probe_socket_create::getBufferPrefix(void)
{
	return "socket-create";
}

void
Probe_socket_create::processBuffer(std::stringstream &buffer, NetUuidData *data)
{
	std::string uuid, pidStr, procArgs;
	std::getline(buffer, uuid, ' ');
	std::getline(buffer, pidStr, ' ');
	std::getline(buffer, procArgs, '\0');
	pid_t pid = stol(pidStr);

	data->pidSocketMap.emplace(pid, uuid);

	typedef std::multimap<std::string, pid_t>::const_iterator it_t;
	std::pair<it_t, it_t> its = data->procNamePidMap.equal_range(procArgs);
	for (it_t it = its.first; it != its.second; ++it)
		if (it->second == pid)
			return;
	data->procNamePidMap.emplace_hint(its.second, procArgs, pid);
}
