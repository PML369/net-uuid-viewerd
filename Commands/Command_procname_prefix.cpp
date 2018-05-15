#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include "json.hpp"
#include <vector>

std::string
Command_procname_prefix::getCommand()
{
	return "procname_prefix";
}

void
Command_procname_prefix::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::string prefix;
	std::getline(sstream, prefix, '\0');

	try
	{
		nlohmann::json j = nlohmann::json::array();
		data->procNamePidTrie.getDataWithKeyPrefix<std::string>(
			prefix.c_str(), prefix.length(),
			std::back_inserter(j),
			[](RbTst<char, PacketInfo>::tKey key,
					std::vector<pid_t> *vec) {
				return std::string(key.first, key.second);
			});
		out += j.dump();
	}
	catch (std::exception &e)
	{
		out += e.what();
	}
}
