#ifndef COMMANDTYPE_HPP
#define COMMANDTYPE_HPP

#include <string>
#include <sstream>

class NetUuidData;

class CommandType
{
public:
	virtual std::string getCommand() = 0;
	virtual void	    execute(std::stringstream&, std::string&,
						NetUuidData *) =0;
};

#endif /*COMMANDTYPE_HPP*/
