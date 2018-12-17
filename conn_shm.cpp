#include "connector.h"

#define BUF_SIZE 150

static void* buf = NULL;
static int shmDesc = 0;

Connector::Connector()
{
	if ((shmDesc = shm_open("/shm", O_CREAT | O_RDWR, S_IRWXU)) == -1)
	{
		syslog(LOG_INFO, "ERROR: shm - open shm");
		shm_unlink("/shm");
	}

	if (ftruncate(shmDesc, BUF_SIZE) == -1)
	{
		syslog(LOG_INFO, "ERROR: shm - ftruncate");	
	}

	buf = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmDesc, 0);    
}

bool Connector::Read(void* rBuf, int rBufSize)
{
	if (rBufSize > BUF_SIZE)
	{
		syslog(LOG_INFO, "ERROR: shm - read buff sizes");	
		return false;
	}

	if ((rBuf = memcpy(rBuf, buf, rBufSize)) == NULL)
	{
   		syslog(LOG_INFO, "ERROR: shm - read memcpy");	
		return false;	
	}
	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{
	if (wBufSize > BUF_SIZE)
	{
		syslog(LOG_INFO, "ERROR: shm - write buff size");	
		return false;
	}

	if ((buf = memcpy(buf, wBuf, wBufSize)) == NULL)
	{
	    	syslog(LOG_INFO, "ERROR: shm - write memcpy");	
		return false;
	}
	return true;
}

Connector::~Connector()
{
	munmap(buf, BUF_SIZE);
	shm_unlink("/shm");
}
