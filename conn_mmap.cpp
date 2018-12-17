#include "connector.h"

#define BUF_SIZE 150
static void* buf = NULL;

Connector::Connector()
{
	buf = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

bool Connector::Read(void* rBuf, int rBufSize)
{
    	if (rBufSize > BUF_SIZE)
	{
        	syslog(LOG_INFO, "ERROR: mmap - read buff size");
		return false;
	}

	if ((rBuf = std::memcpy(rBuf, buf, rBufSize)) == NULL)
	{
	        syslog(LOG_INFO, "ERROR: mmap - read memcpy");
		return false;	
	}
    	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{	
	if (wBufSize > BUF_SIZE)
	{
	        syslog(LOG_INFO, "ERROR: mmap - write buff size");
		return false;
	}

	if ((buf = std::memcpy(buf, wBuf, wBufSize)) == NULL)
	{
        	syslog(LOG_INFO, "ERROR: mmap - write memcpy");
		return false;	
    	}
   	return true;
}

Connector::~Connector()
{
    munmap(buf, BUF_SIZE);
}
