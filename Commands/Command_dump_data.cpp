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
		out += ", address=[";
		bool doneFirst = false;
		char addrbuf[sizeof(void*) * 2 + 3];
		for (std::vector<void *>::iterator itp =
				p->addresses.begin();
				itp != p->addresses.end(); ++itp)
		{
			if (doneFirst)
			{
				out += ", ";
				doneFirst = true;
			}
			snprintf(addrbuf, sizeof(void*) * 2 + 3, "%p", *itp);
			out.append(addrbuf);
		}
		out += "]]\n";
	}
	out += "\n";
}
