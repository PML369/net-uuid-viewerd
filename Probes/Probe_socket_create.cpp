#include "ProbeTypes.hpp"

#include <string>
#include <iostream>
#include <algorithm>

std::string
Probe_socket_create::getDScript(void)
{
	return LONG_STRING_LITERAL(
		net_uuid:socket::create
		{
			printf("socket-create %s %d %s\0",
					uuidtostr((uintptr_t)args[0]),
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

	NetUuidData::pidlist_t *pidVec =
				getOrCreateProcPidVector(procArgs, data);
	if (std::find(pidVec->begin(), pidVec->end(), pid) == pidVec->end())
		pidVec->push_back(pid);
}
