#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>

#include "connector.h"


class Client
{
public:
	Client()
	{
		width = 600;
		height = 400;

		racketWidth = 10;
		racketHeight = 100;

		curPosY = height/2 - racketHeight/2;

		step = 2 * (height / racketHeight); // 2*(400/100)=8
	}

	int genPosY()
        {
		if(curPosY + 2*step > height)
			step = -step;
		if(curPosY + 2*step < 0)
			step = -step;
		curPosY += step;

		return curPosY;
	}

	void run(Connector& connector, sem_t* semHost, sem_t* semClient)
	{
		int end;
		int posY;

    		while (true)
		{
        		sem_wait(semClient);
    			connector.Read(&end, sizeof(int));

    			if (end == 1)
			{
        			sem_close(semClient);
    				sem_unlink("/semClient");
        			break;
        		}
			posY = genPosY();
        		connector.Write(&posY, sizeof(int));
        		sem_post(semHost);		
        	}
        	sem_post(semHost);
        	exit(0);
	}

private:
	int width;
	int height;

	int racketWidth;
	int racketHeight;

	int curPosY;
	int step;
};

#endif
