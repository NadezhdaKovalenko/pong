#include <GL/glut.h>
#include <GL/freeglut.h>
#include <termios.h>
#include <unistd.h>
#include <syslog.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "host.h"
#include "client.h"
#include "connector.h"

static Connector connector;
static Host host;
static Client client;

static sem_t* semHost;
static sem_t* semClient;

int interval = (int)(1000/60);

void update(int value)
{
	host.updateField();
   	glutTimerFunc(interval, update, 0);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    	if(key == 27)
	{
        	host.final = 1;
    	}
}

void enable2D(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	host.drawHost();
	glutSwapBuffers();
}

static void render_init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 400);
	glutCreateWindow("GAME: My Pong");
	glutDisplayFunc(draw);
    	glutKeyboardFunc(keyboard);
	glutTimerFunc(interval, update, 0);
	enable2D(600, 400);
	glColor3f (1.0f, 1.0f, 1.0f);
}

int main(int argc, char** argv)
{
	int pid;

	semHost = sem_open("/semHost", O_CREAT | O_EXCL, S_IRWXU, 0);
	semClient  = sem_open("/semClient", O_CREAT | O_EXCL, S_IRWXU, 0);

	if (semHost == SEM_FAILED || semClient == SEM_FAILED)
	{
	    	sem_unlink("/semHost");
	    	sem_unlink("/semClient");

	    	syslog(LOG_INFO, "Semaphore open error");
	    	exit(1);
	}
      
    	pid = fork();	
 
    	if (pid)
	{     
	        render_init(argc, argv);       
	    	host.run(connector, semHost, semClient); 
    	} 
    	else 
	{  
    		client.run(connector, semHost, semClient);
    	} 
           
	exit(0);
}
