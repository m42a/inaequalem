#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "bullet.h"
#include "enemy.h"

using namespace std;

int ticker;

void render()
{
}

void resize(int w, int h)
{
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

	glutMainLoop();
}
