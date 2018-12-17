#ifndef HOST_H
#define HOST_H

#include <math.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <sstream>
#include <termios.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "connector.h"

class Host
{
public:
	Host();	
  	void updateField();
  	void drawHost();
  	void run(Connector& connector, sem_t* semHost, sem_t* semClient);
	
        int final;

private:
	int width;
	int height;

	int racketWidth;
	int racketHeight;
	int racketHost_x, racketHost_y;
        int racketClient_x, racketClient_y;
	int racketSpeed;

	float ballPos_x, ballPos_y;
	float ballDir_x, ballDir_y;
	int ballSize;
	int ballSpeed;
	
	bool changeClientRacket;
	int newPosYClient;

  	bool isInRacket(int p_x, int p_y);
  	void updateClientRacket(int posY);
	void getNewBallPos();
	void checkLeftWall();
	void checkRightWall();
	void checkTopWall();
	void checkBottomWall();
	void norm(float& x, float &y);
	void drawRect(int p_x, int p_y, float width, float height);
};

#endif
