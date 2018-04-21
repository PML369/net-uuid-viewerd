#include "RequestHandler.hpp"
#include "Commands/CommandTypes.hpp"
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <vector>

#ifndef MAX_WAITING_CONNECTIONS
#define MAX_WAITING_CONNECTIONS 20
#endif

#ifndef ACCEPT_TRIES_PER_CALL
#define ACCEPT_TRIES_PER_CALL 10
#endif

#ifndef RECV_BUF_LEN
#define RECV_BUF_LEN 512
#endif

std::vector<CommandType *> commands = { new Command_dump_data() };

RequestHandler::RequestHandler(unsigned int listenTimeout, NetUuidData *data)
{
	listenTimeout *= (ACCEPT_TRIES_PER_CALL - 1);
	listenTimeout /= ACCEPT_TRIES_PER_CALL;
	this->listenTimeout.tv_sec = listenTimeout / 1000;
	this->listenTimeout.tv_usec = (listenTimeout % 1000) * 1000;

	unsigned int sleepTime = listenTimeout / ACCEPT_TRIES_PER_CALL;
	this->sleepTime = std::chrono::microseconds(
				std::chrono::milliseconds(sleepTime));

	this->netUuidData = data;
	this->listenSocket = -1;
	this->listening = false;
	this->hasAccepted = false;
	this->socketPath = "";
}

bool
RequestHandler::setupUnixSocket(std::string const& socketPath,
					uid_t uid, gid_t gid, mode_t mode)
{
	if (listening)
		return false;

	listenSocket = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (listenSocket == -1)
		return false;

	struct sockaddr_un path;
	memset(&path, 0, sizeof(struct sockaddr_un));
	path.sun_family = AF_UNIX;

	if (socketPath.length() >= (sizeof(path.sun_path))/(sizeof(char)))
		return false;
	strcpy(path.sun_path, socketPath.c_str());
	this->socketPath = socketPath;

	int ret = bind(listenSocket, (struct sockaddr *)&path, sizeof(path));
	if (ret != 0)
		return false;

	chown(socketPath.c_str(), uid, gid);
	chmod(socketPath.c_str(), mode);

	ret = listen(listenSocket, MAX_WAITING_CONNECTIONS);
	listening = (ret == 0);
	return listening;
}

void
RequestHandler::processRequestsFromSocket(void)
{
	if (!listening)
		return;

	struct timeval now, end;
	gettimeofday(&now, NULL);
	timeradd(&now, &listenTimeout, &end);

	if (hasAccepted)
		processRequest();

	for (; timercmp(&now, &end, <); gettimeofday(&now, NULL))
	{
		while (nonBlockingAccept())
		{
			gettimeofday(&now, NULL);
			if (timercmp(&now, &end, >))
				break;
			processRequest();
		}

		gettimeofday(&now, NULL);
		if (timercmp(&now, &end, <))
			std::this_thread::sleep_for(sleepTime);
	}
}

bool
RequestHandler::nonBlockingAccept(void)
{
	if (!listening || hasAccepted)
		return false;

	acceptedSocket = accept(listenSocket, NULL, NULL);
	hasAccepted = (acceptedSocket >= 0);
	return hasAccepted;
}

bool
RequestHandler::readRequest(std::string &request)
{
	request.clear();
	char buffer[RECV_BUF_LEN];
	char *end = NULL;
	unsigned int packetLen = 0;
	size_t requestLength = 0;

	while (end == NULL || request.length() != requestLength)
	{
		if (end == NULL && packetLen == RECV_BUF_LEN-1)
			return false;

		unsigned int bufStart = end == NULL ? packetLen : 0;
		int recvLen = recv(acceptedSocket, &buffer[bufStart],
						RECV_BUF_LEN -1 -bufStart, 0);
		if (recvLen < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				std::this_thread::sleep_for(sleepTime/10);
				continue;
			}
			else
				return false;
		}

		if (end == NULL)
		{
			packetLen += recvLen;
			buffer[packetLen] = '\0';
			end = strchr(buffer, ' ');
			if (end != NULL)
			{
				size_t lengthStrLength = end - buffer;
				size_t bufferLength = packetLen
							-1ULL -lengthStrLength;

				*end = '\0';
				requestLength = std::stoull(buffer);

				size_t readLength = std::min(requestLength,
								bufferLength);
				request.append((++end), readLength);
			}
		}
		else
		{
			size_t readLength = std::min((size_t)recvLen,
						requestLength-request.length());

			request.append(buffer, readLength);
		}
	}

	return true;
}

void
RequestHandler::processRequest(void)
{
	std::string request, output;

	if (readRequest(request))
	{
		handleCommand(request, output);
		writeOutput(output);
	}

	close(acceptedSocket);
	hasAccepted = false;
}

void
RequestHandler::writeOutput(std::string &output)
{
	std::string prefix = std::to_string(output.length()) + " ";
	size_t pLength = prefix.length();
	size_t totalLength = pLength + output.length();
	size_t written = 0;

	while (written < totalLength)
	{
		ssize_t wrote;
		if (written < prefix.length())
			wrote = write(acceptedSocket,
					&(prefix.c_str())[written],
					pLength - written);
		else
			wrote = write(acceptedSocket,
					&(output.c_str())[written-pLength],
					totalLength - written);

		if (wrote >= 0)
			written += wrote;
		else
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				return;
			else
				std::this_thread::sleep_for(sleepTime/10);
	}
}

void
RequestHandler::handleCommand(std::string &command, std::string &output)
{
	std::stringstream sstream(command);
	std::string prefix;
	std::getline(sstream, prefix, ' ');

	for (std::vector<CommandType *>::iterator it = commands.begin();
			it != commands.end(); ++it)
	{
		if (prefix.compare((*it)->getCommand()) == 0)
		{
			(*it)->execute(sstream, output, netUuidData);
			break;
		}
	}
}

RequestHandler::~RequestHandler()
{
	if (hasAccepted)
		close(acceptedSocket);

	if (listening)
		close(listenSocket);

	if (!socketPath.empty())
		unlink(socketPath.c_str());
}
