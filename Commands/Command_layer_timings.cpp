#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include "json.hpp"
#include <vector>

using json = nlohmann::json;

std::string
Command_layer_timings::getCommand()
{
	return "layer_timings";
}

void
Command_layer_timings::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::string layerStr;
	std::getline(sstream, layerStr, '\0');

	try
	{
		json layers = json::parse(layerStr);
		json result = json::array();

		using tKey = RbTst<char, PacketInfo>::tKey;

		for (auto& layer : layers)
		{
			json j;
			j["layer"] = layer;
			std::vector<uint64_t> v;

			bool (*pred)(tKey, PacketInfo*) = NULL;
			uint64_t (*convert)(tKey, PacketInfo*) = NULL;
			if (layer == "IP")
			{
				pred = [](tKey key, PacketInfo *val)
				{
					return val->hasTimeIpArrive &&
						val->hasTimeIpDepart;
				};
				convert = [](tKey key, PacketInfo *val)
				{
					return (val->timeIpDepart -
						val->timeIpArrive)/1000;
				};
			}
			else if (layer == "TCP")
			{
				pred = [](tKey key, PacketInfo *val)
				{
					return val->hasTimeTcpArrive &&
						val->hasTimeTcpDepart;
				};
				convert = [](tKey key, PacketInfo *val)
				{
					return (val->timeTcpDepart -
						val->timeTcpArrive)/1000;
				};
			}
			else if (layer == "UDP")
			{
				pred = [](tKey key, PacketInfo *val)
				{
					return val->hasTimeUdpArrive &&
						val->hasTimeUdpDepart;
				};
				convert = [](tKey key, PacketInfo *val)
				{
					return (val->timeUdpDepart -
						val->timeUdpArrive)/1000;
				};
			}
			else
				continue;

			data->uuidTrie.getDataWithKeyPrefix<uint64_t>(NULL, 0,
					std::back_inserter(v), convert, pred);

			if (v.size() == 0)
				continue;
			j["data"] = v;
			result.push_back(j);
		}

		out += result.dump();
	}
	catch (std::exception &e)
	{
		out += e.what();
	}
}
