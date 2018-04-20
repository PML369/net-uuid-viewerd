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
PROBETYPE(Probe_drop);
PROBETYPE(Probe_socket_create);
/*
net_uuid:packet::fragment
net_uuid:packet::from-socket
net_uuid:packet::to-socket
net_uuid:packet::to-subsys*/

#endif /*PROBETYPES_HPP*/
