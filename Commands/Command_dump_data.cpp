#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include <vector>

std::string
Command_dump_data::getCommand()
{
	return "dump_data";
}

void
Command_dump_data::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::vector<PacketInfo *> allPackets;
	data->uuidTrie.getValuesWithKeyPrefix(NULL, 0,
				std::back_inserter(allPackets));

	out += "Hello world: \n";
	for (std::vector<PacketInfo *>::iterator it = allPackets.begin();
			it != allPackets.end(); ++it)
	{
		PacketInfo *p = *it;
		out += "[uuid=";
		out.append(p->uuid, 36);
		if (p->hasTimeCreated)
		{
			out += ", created=";
			out += std::to_string(p->timeCreated);
		}
		if (p->hasTimeDeleted)
		{
			out += ", deleted=";
			out += std::to_string(p->timeDeleted);
		}
		if (p->wasDropped)
		{
			out += ", droppedAt=";
			out += std::to_string(p->timeDropped);
		}
		if (p->hasTimeIpArrive)
		{
			out += ", ipArrive=";
			out += std::to_string(p->timeIpArrive);
		}
		if (p->hasTimeIpDepart)
		{
			out += ", ipDepart=";
			out += std::to_string(p->timeIpDepart);
		}
		if (p->hasTimeTcpArrive)
		{
			out += ", tcpArrive=";
			out += std::to_string(p->timeTcpArrive);
		}
		if (p->hasTimeTcpDepart)
		{
			out += ", tcpDepart=";
			out += std::to_string(p->timeTcpDepart);
		}
		if (p->hasTimeUdpArrive)
		{
			out += ", udpArrive=";
			out += std::to_string(p->timeUdpArrive);
		}
		if (p->hasTimeUdpDepart)
		{
			out += ", udpDepart=";
			out += std::to_string(p->timeUdpDepart);
		}
		if (p->hasSocketFrom || p->hasSocketTo)
		{
			out += ", socket=";
			out.append(p->socketUuid, 36);
			if (p->hasSocketFrom)
				out += ", hasSocketFrom";
			else
				out += ", hasSocketTo";
		}
		if (!p->subsystemsTouched.empty())
		{
			out += "subsystems=[";
			bool doneFirst = false;
			for (std::vector<std::string>::iterator it =
					p->subsystemsTouched.begin();
					it != p->subsystemsTouched.end(); ++it)
			{
				out += (doneFirst ? ", " : "") + *it;
				doneFirst = true;
			}
			out += "]";
		}
		out += ", address=[";
		bool doneFirst = false;
		char addrbuf[sizeof(void*) * 2 + 3];
		for (std::vector<void *>::iterator itp =
				p->addresses.begin();
				itp != p->addresses.end(); ++itp)
		{
			if (doneFirst)
				out += ", ";
			snprintf(addrbuf, sizeof(void*) * 2 + 3, "%p", *itp);
			out.append(addrbuf);
			doneFirst = true;
		}
		out += "]]\n";
	}

	for (std::multimap<std::string, pid_t>::iterator it =
			data->procNamePidMap.begin();
			it != data->procNamePidMap.end(); ++it)
	{
		out += "[proc=\"";
		out += it->first;
		out += "\", pid=";
		out += std::to_string(it->second);
		out += "]\n";
	}
	for (std::multimap<pid_t, std::string>::iterator it =
			data->pidSocketMap.begin();
			it != data->pidSocketMap.end(); ++it)
	{
		out += "[pid=";
		out += std::to_string(it->first);
		out += ", socket=";
		out += it->second;
		out += "]\n";
	}

	out += "\n";
}
