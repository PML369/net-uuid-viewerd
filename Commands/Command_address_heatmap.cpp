#include "CommandTypes.hpp"
#include "NetUuidData.hpp"

#include <vector>

std::string
Command_address_heatmap::getCommand()
{
	return "address_heatmap";
}

static std::pair<std::string, unsigned int>
dataCallback(RbtTrie<char, NetUuidData::infolist_t>::tKey key,
					NetUuidData::infolist_t *value)
{
	return std::pair<std::string, unsigned int>(std::string(key.first, key.second), value->size());
}

void
Command_address_heatmap::execute(std::stringstream& sstream, std::string& out,
				NetUuidData *data)
{
	std::string prefix;
	std::getline(sstream, prefix, ' ');

	if (prefix.substr(0, 2) == "0x")
		prefix = prefix.substr(2);

	std::map<std::string, unsigned int> addrData;
	data->addrTrie.getDataWithKeyPrefix(prefix.c_str(), prefix.length(),
				std::inserter(addrData, addrData.begin()),
				&dataCallback);
	if (addrData.size() == 0)
		return;

	out += "{";

	unsigned int maxRows = 16, rowsFound = 0;
	size_t addrPrefixLen = 1;
	size_t firstLen;
	bool notEnoughRows = false;
	bool first = true;
	for (; !notEnoughRows && rowsFound < maxRows; addrPrefixLen++)
	{
		std::string lastKey = "";
		rowsFound = 0;
		for (std::map<std::string, unsigned int>::iterator
			it = addrData.begin(); it != addrData.end(); ++it)
		{
			if (first)
			{
				firstLen = it->first.length();
				first = false;
			}
			if (it->first.length() == addrPrefixLen)
			{
				notEnoughRows = true;
				break;
			}

			std::string key = it->first.substr(0, addrPrefixLen);
			if (key.compare(lastKey) != 0)
			{
				rowsFound++;
				lastKey = key;
			}
		}
	}
	if (addrPrefixLen < 2)
		addrPrefixLen = 0;
	else
		addrPrefixLen -= 2;
	
	out += "\"zdata\": [";
	
	char hexVals[] = "0123456789abcdef";
	std::vector<std::string> rows;
	std::string currentPrefix = "";
	unsigned short currentHex = 0;
	unsigned int currentCount = 0;
	bool firstRow = true;
	for (std::map<std::string, unsigned int>::iterator
			it = addrData.begin(); it != addrData.end(); ++it)
	{
		std::string key = it->first.substr(0, addrPrefixLen);
		char col = it->first.substr(addrPrefixLen, 1)[0];
		if (key.compare(currentPrefix) != 0)
		{
			if (currentPrefix.length() == 0)
				out += "[";
			else
			{
				for (; currentHex < 16; currentHex++)
				{
					if (currentHex != 0)
						out += ", ";
					else if (!firstRow)
						out += "],\n [";
					out += std::to_string(currentCount);
					currentCount = 0;
				}
				firstRow = false;
				rows.push_back(currentPrefix);
			}
			currentHex = 0;
			currentPrefix = key;
		}
		if (col == hexVals[currentHex])
			currentCount += it->second;
		else
		{
			for (; col != hexVals[currentHex]; currentHex++)
			{
				if (currentHex != 0)
					out += ", ";
				else if (!firstRow)
					out += "],\n [";
				out += std::to_string(currentCount);
				currentCount = 0;
			}
			currentCount = it->second;
		}
	}
	
	for (; currentHex < 16; currentHex++)
	{
		if (currentHex == 0)
			out += "],\n [";
		else
			out += ", ";
		out += std::to_string(currentCount);
		currentCount = 0;
	}
	rows.push_back(currentPrefix);

	out += "]], \"ydata\": [";

	bool doneFirst = false;
	for (std::vector<std::string>::iterator it = rows.begin();
							it != rows.end(); ++it)
	{
		if (doneFirst)
			out += ", ";
		out += "\"0x" + *it + "\"";
		doneFirst = true;
	}
	out += "], \"numZeros\": ";
	out += std::to_string(firstLen - addrPrefixLen - 1);

	out += "}";
}
