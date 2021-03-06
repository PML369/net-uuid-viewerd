#include "dtrace_import.hpp"
#include "Probes/ProbeTypes.hpp"

#include <vector>
#include <sstream>

std::vector<ProbeType *> probes = { new Probe_trace_start(),
				    new Probe_trace_stop(),
				    new Probe_mem_alloc(),
				    new Probe_drop(),
				    new Probe_socket_create(),
				    new Probe_from_socket(),
				    new Probe_to_socket(),
				    new Probe_to_subsys(),
				    new Probe_fragment(),
				    new Probe_layer_arrive(),
				    new Probe_layer_depart()
				};

static dtrace_hdl_t* g_dtp;

static int chewrec (const dtrace_probedata_t *data,
		const dtrace_recdesc_t *rec, void *arg)
{
	// A NULL rec indicates that we've processed the last record.
	if (rec == NULL)
		return (DTRACE_CONSUME_NEXT);
	return (DTRACE_CONSUME_THIS);
}

static int buf_read(const dtrace_bufdata_t *buf, void *arg)
{
	std::stringstream sstream(buf->dtbda_buffered);
	std::string prefix;
	std::getline(sstream, prefix, ' ');

	for (std::vector<ProbeType *>::iterator it = probes.begin();
			it != probes.end(); ++it)
		if (prefix.compare((*it)->getBufferPrefix()) == 0)
			(*it)->processBuffer(sstream, (NetUuidData *)arg);
	return 0;
}

static int handle_dtrace_error(const dtrace_errdata_t *data, void *value)
{
	fprintf(stderr, "DTrace error: %s\n", data->dteda_msg);
	return DTRACE_HANDLE_OK;
}

int applicationInterrupted;

bool setupDTrace(unsigned int rate, NetUuidData *data)
{
	int err;

	if ((g_dtp = dtrace_open(DTRACE_VERSION, 0, &err)) == NULL)
	{
		fprintf(stderr, "failed to initialize dtrace: %s\n", 
				dtrace_errmsg(NULL, err));
		return false;
	}
	printf("Dtrace initialized\n");

	(void) dtrace_setopt(g_dtp, "bufsize", "256m");
	(void) dtrace_setopt(g_dtp, "aggsize", "256m");
	(void) dtrace_setopt(g_dtp, "switchrate",
					(std::to_string(rate) + "ms").c_str());
	dtrace_handle_buffered(g_dtp, &buf_read, (void *)data);
	dtrace_handle_err(g_dtp, &handle_dtrace_error, NULL);
	printf("dtrace options set\n");
	return true;
}

bool installProbes(void)
{
	std::string dScript;
	for (std::vector<ProbeType *>::iterator it = probes.begin();
			it != probes.end(); ++it)
		dScript += (*it)->getDScript();

	dtrace_prog_t *prog = dtrace_program_strcompile(g_dtp, dScript.c_str(),
					DTRACE_PROBESPEC_NAME, 0, 0, NULL);
	if (prog == NULL)
	{
		fprintf(stderr, "Failed to compile DTrace program.\n%s\n%s",
				"Failed program was:", dScript.c_str());
		return false;
	}
	printf("dtrace program compiled\n");

	dtrace_proginfo_t info;
	if (dtrace_program_exec(g_dtp, prog, &info) == -1)
	{
		fprintf(stderr, "Failed to enable dtrace probes.\n");
		return false;
	}
	printf("dtrace probes enabled\n");

	if (dtrace_go(g_dtp) != 0)
	{
		fprintf(stderr, "could not start instrumentation\n");
		return false;
	}
	printf("instrumentation started\n");

	return true;
}

bool processDTraceBuffers(void)
{
	switch (dtrace_work(g_dtp, NULL, NULL, chewrec, NULL))
	{
		case DTRACE_WORKSTATUS_DONE:
			applicationInterrupted = 1;
			return true;
		case DTRACE_WORKSTATUS_OKAY:
			return true;
		case DTRACE_WORKSTATUS_ERROR:
			fprintf(stderr, "Processing aborted with error: %s\n",
					dtrace_errmsg(g_dtp,
						dtrace_errno(g_dtp)));
			return false;
	}
}

void sleepDTrace(void)
{
	dtrace_sleep(g_dtp);
}

bool stopTracing(void)
{
	if (dtrace_stop(g_dtp) == -1)
	{
		fprintf(stderr, "Could not stop tracing\n");
		return false;
	}
	return true;
}

void closeDTrace(void)
{
	dtrace_close(g_dtp);
}
