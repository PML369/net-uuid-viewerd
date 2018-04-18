#include "NetUuidData.hpp"
#include <sys/time.h>
#include <string>
#include <chrono>

class RequestHandler
{
public:
	RequestHandler(unsigned int listenTimeout, NetUuidData *data);
	bool setupUnixSocket(std::string const& socketPath);
	void processRequestsFromSocket(void);
	~RequestHandler();

private:
	bool nonBlockingAccept(void);
	void processRequest(void);

	bool readRequest(std::string &request);
	void handleCommand(std::string &command, std::string& output);
	void writeOutput(std::string &output);

	int listenSocket;
	int acceptedSocket;
	std::string socketPath;

	bool listening;
	bool hasAccepted;

	struct timeval listenTimeout;
	std::chrono::microseconds sleepTime;

	NetUuidData *netUuidData;
};
