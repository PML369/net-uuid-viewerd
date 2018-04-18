#include <signal.h>
#include "dtrace_import.hpp"
#include "RequestHandler.hpp"

static void intr (int signo)
{
	applicationInterrupted = 1;
}

void installInterruptHandler()
{
	struct sigaction act;
	(void) sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = intr;
	(void) sigaction(SIGINT, &act, NULL);
	(void) sigaction(SIGTERM, &act, NULL);
}

int main (int argc, char** argv)
{
	unsigned int loopRate = 100;
	NetUuidData data;
	RequestHandler handler(loopRate, &data);

	if (!setupDTrace(loopRate, &data))
		return -1;

	if (!installProbes())
		return -2;

	if (!handler.setupUnixSocket("/tmp/net-uuid.sock"))
	{
		closeDTrace();
		return -3;
	}

	installInterruptHandler();

	while (true)
	{
		if (applicationInterrupted)
			break;

		handler.processRequestsFromSocket();
		sleepDTrace();

		if (!processDTraceBuffers())
			break;
	}

	if (!stopTracing())
		return -4;

	if (!processDTraceBuffers())
		return -5;

	printf("closing dtrace\n");
	closeDTrace();
	return 0;
}
