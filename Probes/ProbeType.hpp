#ifndef PROBETYPE_HPP
#define PROBETYPE_HPP

#include <string>
#include <sstream>

#include <NetUuidData.hpp>

#define LONG_STRING_LITERAL(...) #__VA_ARGS__

class PacketInfo;

class ProbeType
{
public:
	virtual std::string getDScript() = 0;
	virtual std::string getBufferPrefix() = 0;
	virtual void	    processBuffer(std::stringstream&, NetUuidData *) =0;
protected:
	PacketInfo *	         getOrCreateEntry(std::string &, NetUuidData *);
	NetUuidData::infolist_t *getOrCreateAddressVector(std::string &,
								NetUuidData *);
	NetUuidData::infolist_t *getOrCreateSocketVector(std::string &,
								NetUuidData *);
	NetUuidData::pidlist_t  *getOrCreateProcPidVector(std::string &,
								NetUuidData *);
private:
	template <typename T>
	T *getOrCreateVector(std::string &, RbTst<char, T> *trie);
};

#endif /*PROBETYPE_HPP*/
