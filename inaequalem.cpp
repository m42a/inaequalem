#include <iostream>
#include <array>
#include <algorithm>

#include "inaequalem.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

using namespace std;

const float ratio=640.0/480.0; //The desired width/height ratio
const float textheight=119.05; //From the documentation
const float textlineheight=119.05+33.33; //From the documentation

//static bool directions[4]={false,false,false,false}; //Up down left right
int direction=0;

int ticker=0;
player p(.5, .5);

void writetext(float x, float y, float size, const string &s)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size/textheight, size/textheight, size/textheight);
	for_each(s.cbegin(), s.cend(), [](char c){glutStrokeCharacter(GLUT_STROKE_ROMAN, c);});
	glPopMatrix();
}

void drawBackground()
{
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0,0,1,1);
}

void drawSidepanel()
{
	glColor3f(0.2, 0.2, 0.2);
	glRectf(1,0,ratio,1);
	glColor3f(1.0, 1.0, 1.0);
	writetext(1.1, .9, .05, "Score");
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	p.draw();
	drawSidepanel();
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
	glutTimerFunc(16, gamelogic, 0);
	//cout << "logic\n";
	if (direction==-1)
		p.move(player::right);
	else if (direction==2)
		p.move(player::upright);
	else if (direction==3)
		p.move(player::up);
	else if (direction==4)
		p.move(player::upleft);
	else if (direction==1)
		p.move(player::left);
	else if (direction==-2)
		p.move(player::downleft);
	else if (direction==-3)
		p.move(player::down);
	else if (direction==-4)
		p.move(player::downright);
	++ticker;
	glutPostRedisplay();
}

void keydown(unsigned char key, int x, int y)
{
}

void keyup(unsigned char key, int x, int y)
{
}

void specialdown(int key, int x, int y)
{
	if (key==GLUT_KEY_UP)
		direction+=3;
	else if (key==GLUT_KEY_DOWN)
		direction-=3;
	else if (key==GLUT_KEY_LEFT)
		direction+=1;
	else if (key==GLUT_KEY_RIGHT)
		direction-=1;
	//direction=3*(directions[0]-directions[1])+directions[2]-directions[3];
}

void specialup(int key, int x, int y)
{
	if (key==GLUT_KEY_UP)
		direction-=3;
	else if (key==GLUT_KEY_DOWN)
		direction+=3;
	else if (key==GLUT_KEY_LEFT)
		direction-=1;
	else if (key==GLUT_KEY_RIGHT)
		direction+=1;
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
