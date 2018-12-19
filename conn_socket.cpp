#include "connector.h"

static int sockets[2];

Connector::Connector()
{
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1)
  {
		syslog(LOG_INFO, "ERROR: sockets - connector");
	}
}

bool Connector::Read(void* rBuf, int rBufSize)
{
	if (read(sockets[0], rBuf, rBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: sockets - read");
		return false;
	}
	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{
	if (write(sockets[1], wBuf, wBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: sockets - write");
		return false;
	}
	return true;
}

Connector::~Connector()
{
	close(sockets[0]);
	close(sockets[1]);
}
