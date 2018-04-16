#ifndef PROBETYPES_HPP
#define PROBETYPES_HPP

#include "ProbeType.hpp"

class Probe_trace_start : public ProbeType
{
public:
	std::string getDScript();
	std::string getBufferPrefix();
	void	    processBuffer(std::stringstream&, NetUuidData *);
};
/*net_uuid:packet::trace-stop
net_uuid:mem::alloc
net_uuid:packet::fragment
net_uuid:packet::from-socket
net_uuid:packet::to-socket
net_uuid:packet::to-subsys
net_uuid:packet::drop*/

#endif /*PROBETYPES_HPP*/
