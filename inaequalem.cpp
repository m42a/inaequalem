#include <iostream>
#include <array>

#include "inaequalem.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

using namespace std;

const float ratio=640.0/480.0; //The desired width/height ratio

int ticker=0;
player p(.5, .5);

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0,0,1,1);
	glColor3f(0.2, 0.2, 0.2);
	glRectf(1,0,ratio,1);
	p.draw();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (w==0 || h==0)
		return;
	if (ratio*h>w)
		glViewport(0, (h-w/ratio)/2, w, w/ratio);
	else
		glViewport((w-h*ratio)/2, 0, h*ratio, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,ratio,0,1);
	glMatrixMode(GL_MODELVIEW);
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
	if (key==GLUT_KEY_DOWN)
		p.move(player::down);
	if (key==GLUT_KEY_UP)
		p.move(player::up);
	glutPostRedisplay();
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

	glClearColor(1,1,1,0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

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


	glutMainLoop();
}
