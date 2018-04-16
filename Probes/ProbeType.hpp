#ifndef PROBETYPE_HPP
#define PROBETYPE_HPP

#include <string>
#include <sstream>

#define LONG_STRING_LITERAL(...) #__VA_ARGS__

class NetUuidData;

class ProbeType
{
public:
	virtual std::string getDScript() = 0;
	virtual std::string getBufferPrefix() = 0;
	virtual void	    processBuffer(std::stringstream&, NetUuidData *) =0;
};

#endif /*PROBETYPE_HPP*/
