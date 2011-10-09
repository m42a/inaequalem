#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

#include "inaequalem.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

using namespace std;

const float ratio=640.0/480.0; //The desired width/height ratio
const float textheight=119.05; //From the documentation
const float textlineheight=119.05+33.33; //From the documentation

int movedir=0;
bool shooting;

struct timespec then;
int ticker=0;
string fps;

player p(.5, .5);
vector<bullet> pb;

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
	writetext(1.02, .9, .04, "Score: &e0");
	writetext(1.02, .5, .02, strprintf("pb.size()=%d",pb.size()));
	if (pb.size()!=0)
		writetext(1.02, .5-.03*textlineheight/textheight, .02, strprintf("pb[0].y=%f",pb[0].y));
	writetext(1.02, .02, .03, fps);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	p.draw();
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&bullet::draw));
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

template<class T>
void stepandcull(vector<T> &v)
{
	for_each(v.begin(), v.end(), mem_fun_ref(&T::step));
	auto i=remove_if(v.begin(), v.end(), mem_fun_ref(&T::isdestroyed));
	v.erase(i, v.end());
}

void gamelogic(int v)
{
	glutTimerFunc(16, gamelogic, 0);
	//cout << "logic\n";
	if (movedir==-1)
		p.move(direction::right);
	else if (movedir==2)
		p.move(direction::upright);
	else if (movedir==3)
		p.move(direction::up);
	else if (movedir==4)
		p.move(direction::upleft);
	else if (movedir==1)
		p.move(direction::left);
	else if (movedir==-2)
		p.move(direction::downleft);
	else if (movedir==-3)
		p.move(direction::down);
	else if (movedir==-4)
		p.move(direction::downright);
	stepandcull(pb);
	++ticker;
	if (ticker%4==0 && shooting)
		pb.emplace_back(p.x, p.y, 0, direction::up, .02);
	if (ticker%32==0)
	{
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		double diff=(now.tv_sec-then.tv_sec)+(now.tv_nsec-then.tv_nsec)/1000000000.0;
		double frames=32/diff;
		int size=snprintf(NULL, 0, "FPS: %05.2f", frames);
		char *s=new char[size+1];
		sprintf(s, "FPS: %05.2f", frames);
		fps=s;
		delete[] s;
		then=now;
	}
	glutPostRedisplay();
}

void keydown(unsigned char key, int x, int y)
{
	if (key=='z')
		shooting=true;
}

void keyup(unsigned char key, int x, int y)
{
	if (key=='z')
		shooting=false;
}

void specialdown(int key, int x, int y)
{
	if (key==GLUT_KEY_UP)
		movedir+=3;
	else if (key==GLUT_KEY_DOWN)
		movedir-=3;
	else if (key==GLUT_KEY_LEFT)
		movedir+=1;
	else if (key==GLUT_KEY_RIGHT)
		movedir-=1;
}

void specialup(int key, int x, int y)
{
	if (key==GLUT_KEY_UP)
		movedir-=3;
	else if (key==GLUT_KEY_DOWN)
		movedir+=3;
	else if (key==GLUT_KEY_LEFT)
		movedir-=1;
	else if (key==GLUT_KEY_RIGHT)
		movedir+=1;
}

void mousemove(int x, int y)
{
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Not touhou");

	clock_gettime(CLOCK_MONOTONIC, &then);

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
