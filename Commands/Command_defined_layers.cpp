#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

std::string
Command_defined_layers::getCommand()
{
	return "defined_layers";
}

void
Command_defined_layers::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	out += "[ \"IP\", \"TCP\", \"UDP\" ]";
}
