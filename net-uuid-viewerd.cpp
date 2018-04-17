#include <signal.h>
#include "dtrace_import.hpp"

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

	if (!setupDTrace(loopRate, &data))
		return -1;

	if (!installProbes())
		return -2;

	installInterruptHandler();

	while (true)
	{
		if (applicationInterrupted)
			break;

		sleepDTrace();

		if (!processDTraceBuffers())
			break;
	}

	if (!stopTracing())
		return -3;

	if (!processDTraceBuffers())
		return -4;

	printf("closing dtrace\n");
	closeDTrace();
	return 0;
}
