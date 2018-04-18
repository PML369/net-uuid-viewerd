#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include <iterator>

class AppendPacketDataToString
{
public:
	struct output_iterator_tag {};

	AppendPacketDataToString(std::string &string)
	{
		output = &string;
	}

	AppendPacketDataToString &operator++(void)
	{
		(*output) += current;
		current.empty();
		return *this;
	}
	AppendPacketDataToString operator++(AppendPacketDataToString &it)
	{
		output += current;
		current.empty();
		return *this;
	}

	void operator*(RbtTrie<char, PacketData>::tKey key)
	{
		current(key.first, key.second);
	}
private:
	std::string *output;
	std::string current;
}

std::string
Command_dump_data::getCommand()
{
	return "dump_data";
}

void
Command_dump_data::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	AppendPacketDataToString it(out);
	data->uuidTrie.getKeysWithPrefix(NULL, 0, it);

	out += "Hello world: \n";
	out += sstream.str();
	out += "\n";
}
