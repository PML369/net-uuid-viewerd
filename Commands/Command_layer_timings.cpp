#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

std::string
Command_layer_timings::getCommand()
{
	return "layer_timings";
}

void
Command_layer_timings::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::string layers;
	std::getline(sstream, layers, '\0');
	out += "[ \"IP\", \"TCP\", \"UDP\" ]";
}
