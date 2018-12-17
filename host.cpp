#include "host.h"

Host::Host()
{
	width = 600;
	height = 400;

	racketWidth = 10;
	racketHeight = 100;
	racketSpeed = 5;

	racketHost_x = racketWidth;
	racketHost_y = height/2 - racketHeight/2;
	
	racketClient_x = width - racketWidth;
	racketClient_y = height/2 - racketHeight/2;

	newPosYClient = 0;

	ballPos_x = width / 2;
	ballPos_y = height / 2;
	ballDir_x = -1.0f;
	ballDir_y = 0.1f;
	ballSize = 20;
	ballSpeed = 2;

	changeClientRacket = true;
	final = 0;
}

void Host::norm(float& x, float &y)
{
	float length = sqrt((x * x) + (y * y));

	if (length != 0.0f)
	{
		x = x / length;
		y = y / length;
	}
}

void Host::drawRect(int p_x, int p_y, float width, float height)
{
	glBegin (GL_QUADS);
	glVertex2f (p_x - width / 2, p_y - height / 2);
	glVertex2f (p_x - width / 2, p_y + height / 2);
	glVertex2f (p_x + width / 2, p_y + height / 2);
	glVertex2f (p_x + width / 2, p_y - height / 2);
	glEnd ();
}

void Host::drawHost()
{
	drawRect(racketHost_x, racketHost_y, racketWidth, racketHeight);
	drawRect(racketClient_x, racketClient_y, racketWidth, racketHeight);
	drawRect(ballPos_x, ballPos_y, ballSize, ballSize);
}

bool Host::isInRacket(int p_x, int p_y)
{
	return ballPos_x<(p_x+racketWidth/2) &&
		ballPos_x>(p_x-racketWidth/2) &&
		ballPos_y<(p_y+racketHeight/2) &&
		ballPos_y>p_y-racketHeight/2;
}

void Host::getNewBallPos()
{
	ballPos_x += ballDir_x * ballSpeed;
	ballPos_y += ballDir_y * ballSpeed;
}

void Host::checkLeftWall()
{
	if (ballPos_x - ballSize / 2 <= 0)
	{
		ballPos_x = width / 2;
		ballPos_y = height / 2;
		
		ballDir_x = fabs(ballDir_x); 
		ballDir_y = 0;
		
		changeClientRacket = true;
	}
}

void Host::checkRightWall()
{
	if (ballPos_x + ballSize / 2 >= width)
	{
		ballPos_x = width / 2;
		ballPos_y = height / 2;
		
		ballDir_x = -fabs(ballDir_x); 
		ballDir_y = 0;
		
		changeClientRacket = true;
	}
}

void Host::checkTopWall()
{
	if (ballPos_y + ballSize / 2 > height)
	{
		ballDir_y = -fabs(ballDir_y); 
	}
}

void Host::checkBottomWall()
{
	if (ballPos_y - ballSize / 2 < 0)
	{
		ballDir_y = fabs(ballDir_y); 
	}
}

void Host::updateField() 
{
	// ball racket
	changeClientRacket = false;
	getNewBallPos();

	if(isInRacket(racketHost_x, racketHost_y))
	{
		float t = ((ballPos_y - racketHost_y) / racketHeight) - 0.5f;
		ballDir_x = fabs(ballDir_x);
		ballDir_y = t;
		changeClientRacket = true;
	}

	if (isInRacket(racketClient_x, racketClient_y))
	{
		float t = ((ballPos_y - racketClient_y) / racketHeight) - 0.5f;
		ballDir_x = -fabs(ballDir_x);
		ballDir_y = t;
	}

	checkLeftWall();
	checkRightWall();
	checkTopWall();
	checkBottomWall();

	//norm ball direction
	norm(ballDir_x, ballDir_y);

	//host racket
	if(ballPos_x < width / 2 && ballDir_x <= 0)
	{
		if(racketHost_y >= ballPos_y)
		{
			racketHost_y -= 1;
		}
		if(racketHost_y <= ballPos_y)
		{
			racketHost_y += 1;	
		}
	}
	
	if(racketHost_y + racketHeight / 2 > height)
	{
		racketHost_y -= racketHeight / 2;
	}
	
	if(racketHost_y - racketHeight / 2 < 0)
	{
		racketHost_y += racketHeight / 2;
	}

	//client racket
	if(ballPos_x > width / 2 && ballDir_x >= 0)
	{
		if(racketClient_y < newPosYClient)
		{
			racketClient_y += 1;	
		}
		if(racketClient_y > newPosYClient)
		{
			racketClient_y -= 1;
		}
	}
}

void Host::updateClientRacket(int posY)
{
	if(changeClientRacket)
	{
		newPosYClient = posY;
		changeClientRacket = false;
	}
}

void Host::run(Connector& connector, sem_t* semHost, sem_t* semClient)
{
	int newPosY;

	while (true)
	{      
        	connector.Write(&final, sizeof(int));
        	sem_post(semClient);

	        if(final == 1)
		{       	
	       		sem_close(semHost);
	        	sem_unlink("/semHost");
	     		break;	
	     	}
	     	sem_wait(semHost); 
	         
	        connector.Read(&newPosY, sizeof(int));        
	        updateClientRacket(newPosY);
	     	glutMainLoopEvent();  	
	}

    	sem_post(semClient);
	exit(0);
}
