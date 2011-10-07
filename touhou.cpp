#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "bullet.h"
#include "enemy.h"
#include "player.h"

using namespace std;

int ticker;

player p(.5, .5);

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
	glRectf(0,0,1,1);
	p.draw();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (w==0 || h==0)
		return;
	const float ratio=4.0/3.0; //The desired width/height ratio
	if (ratio*h>w)
		glViewport(0, (h-w/ratio)/2, w, w/ratio);
	else
		glViewport((w-h*ratio)/2, 0, h*ratio, h);
	gluOrtho2D(0,ratio,0,1);
	//glutPostRedisplay();
}

void gamelogic(int v)
{
}

void keydown(unsigned char key, int x, int y)
{
}

void keyup(unsigned char key, int x, int y)
{
}

void specialdown(int key, int x, int y)
{
}

void specialup(int key, int x, int y)
{
}

void mousemove(int x, int y)
{
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Not touhou");

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutTimerFunc(16, gamelogic, 0);
	glutIgnoreKeyRepeat(true);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(specialdown);
	glutSpecialUpFunc(specialup);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(mousemove);

	glClearColor(1,1,1,0);

	glutMainLoop();
}
