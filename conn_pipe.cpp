#include "connector.h"

static int pipeDesc[2];

Connector::Connector()
{
	if (pipe(pipeDesc))
	{
		syslog(LOG_INFO, "ERROR: pipe - connector");
	}
}

bool Connector::Read(void* rBuf, int rBufSize)
{
	if (read(pipeDesc[0], rBuf, rBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: pipe - read");
		return false;
	}
	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{
	if (write(pipeDesc[1], wBuf, wBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: pipe - write");
		return false;
	}
	return true;
}

Connector::~Connector()
{
	close(pipeDesc[0]);
	close(pipeDesc[1]);
}
