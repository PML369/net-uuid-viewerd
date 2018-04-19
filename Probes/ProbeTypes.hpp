#ifndef PROBETYPES_HPP
#define PROBETYPES_HPP

#include "ProbeType.hpp"

#define PROBETYPE(p) \
class p : public ProbeType \
{ \
public: \
	std::string getDScript(); \
	std::string getBufferPrefix(); \
	void	    processBuffer(std::stringstream&, NetUuidData *); \
}

PROBETYPE(Probe_trace_start);
PROBETYPE(Probe_trace_stop);
PROBETYPE(Probe_mem_alloc);
/*
net_uuid:packet::fragment
net_uuid:packet::from-socket
net_uuid:packet::to-socket
net_uuid:packet::to-subsys
net_uuid:packet::drop*/

#endif /*PROBETYPES_HPP*/
