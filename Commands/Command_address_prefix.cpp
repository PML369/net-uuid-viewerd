#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include "json.hpp"
#include <vector>

std::string
Command_address_prefix::getCommand()
{
	return "address_prefix";
}

void
Command_address_prefix::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::string prefix;
	std::getline(sstream, prefix, ' ');

	try
	{
		nlohmann::json j = nlohmann::json::array();
		data->addrTrie.getDataWithKeyPrefix<std::string>(
			prefix.c_str(), prefix.length(),
			std::back_inserter(j),
			[](RbTst<char, PacketInfo>::tKey key,
					std::vector<PacketInfo *> *vec) {
				return std::string(key.first, key.second);
			});
		out += j.dump();
	}
	catch (std::exception &e)
	{
		out += e.what();
	}
}
