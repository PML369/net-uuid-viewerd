#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

std::string
Command_dump_data::getCommand()
{
	return "dump_data";
}

void
Command_dump_data::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	out += "Hello world: \n";
	out += sstream.str();
	out += "\n";
}
