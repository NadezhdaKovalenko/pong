#include "connector.h"

static mqd_t msgQ;

Connector::Connector()
{
	mq_attr attr;

	attr.mq_flags   = 0;
	attr.mq_maxmsg  = 1;
	attr.mq_curmsgs = 0;
	attr.mq_msgsize = sizeof(int);

	if ((msgQ = mq_open("/mq", O_CREAT | O_RDWR|O_NONBLOCK, S_IRWXU, &attr)) == -1)
	{
	    	syslog(LOG_INFO, "ERROR: mq -  connector");
	}
}

bool Connector::Read(void* rBuf, int rBufSize)
{
	unsigned int p;

	if (mq_receive(msgQ, (char *)rBuf, rBufSize, &p) == -1)
	{
		syslog(LOG_INFO, "ERROR: mq - read");
		return false;
	}
	return true;
}

bool Connector::Write(void* wBuf, int wBufSize)
{	
	if (mq_send(msgQ, (char *)wBuf, wBufSize, 1) == -1)
	{
	        syslog(LOG_INFO, "ERROR: mq -  write");
		return false;
	}
	return true;
}

Connector::~Connector()
{
	mq_close(msgQ);
}
