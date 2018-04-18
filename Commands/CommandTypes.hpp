#ifndef COMMANDTYPES_HPP
#define COMMANDTYPES_HPP

#include "CommandType.hpp"

#define COMMANDTYPE(p) \
class p : public CommandType \
{ \
public: \
	std::string getCommand(); \
	void	    execute(std::stringstream&, std::string&, NetUuidData *); \
}

COMMANDTYPE(Command_dump_data);

#endif /*COMMANDTYPES_HPP*/
