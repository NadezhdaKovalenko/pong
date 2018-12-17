#include "connector.h"

static int fifoFileDesc = 0;

Connector::Connector()
{
	if (mkfifo("fifo", S_IRWXU) == -1)
	{
		syslog(LOG_INFO, "ERROR: fifo - make fifo");
	}
	
	if ((fifoFileDesc = open("fifo", O_RDWR | O_NONBLOCK)) == -1 )
	{
		syslog(LOG_INFO, "ERROR: fifo - open fifo");	
	}
}

bool Connector::Read(void* rBuf, int rBufSize)
{
	if (read(fifoFileDesc, rBuf, rBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: fifo - read");
		return false;
	}
	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{
	if (write(fifoFileDesc, wBuf, wBufSize) == -1)
	{
		syslog(LOG_INFO, "ERROR: fifo - write");
		return false;
	}
	return true;
}

Connector::~Connector()
{
	close(fifoFileDesc);
	unlink("fifo");
}
