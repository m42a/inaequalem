#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <exception>

#include "inaequalem.h"
#include "entity.h"
#include "player.h"
#include "parse.h"
#include "camera.h"
#include "texture.h"

using namespace std;

const float ratio=640.0/480.0; //The desired width/height ratio
const float textheight=119.05; //From the documentation
const float textlineheight=119.05+33.33; //From the documentation
camera my_camera;
// These should be part of the player class
int movedir=0;
//bool shooting;
int ticker=0;
int background[2];

int widthoffset;
int heightoffset;
int stagewidth;
int sidewidth;

struct timespec then;
string fps; //This is the string to display as an FPS counter, do not rely on it being parsable

//This is us and our bullets
player p(.5, .5);
vector<entity> pb;
//These are the enemies and their bullets
vector<entity> e;
//These are things that will happen, ordered by tick
unordered_multimap<int, entity> lb;
unordered_multimap<int, entity> le;

//Write a string somewhere at a certain size.  Text defaults to the window
//height, so try making the size somewhat small.  Text scales vertically and
//horizontally by the same factor; I doubt there's a need to have squished or
//elongated text.
void writetext(float x, float y, float size, const string &s)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size/textheight, size/textheight, size/textheight);
	//Sadly, C++ lacks currying, so we'll have to make do with a lambda (or bind, but I don't wanna)
	for_each(s.cbegin(), s.cend(), [](char c){glutStrokeCharacter(GLUT_STROKE_ROMAN, c);});
	glPopMatrix();
}

void drawBackground()
{
	//Do something cool with stars
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	//int background=loadRGBtexture("background.dat", 512, 2048);
	glBindTexture(GL_TEXTURE_2D, background[0]);


	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, ticker/1200.0);
		glVertex3f(0,0,0);
		glTexCoord2f(1, ticker/1200.0);
		glVertex3f(1,0,0);
		glTexCoord2f(1, .25+ticker/1200.0);
		glVertex3f(1,1,0);
		glTexCoord2f(0, .25+ticker/1200.0);
		glVertex3f(0,1,0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, background[1]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, ticker/1000.0);
		glVertex3f(0,0,0);
		glTexCoord2f(1, ticker/1000.0);
		glVertex3f(1,0,0);
		glTexCoord2f(1, .25+ticker/1000.0);
		glVertex3f(1,1,0);
		glTexCoord2f(0, .25+ticker/1000.0);
		glVertex3f(0,1,0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void positionCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5, -5, 5, 5, 1, 5);
	glMatrixMode(GL_MODELVIEW);
	//place camera
	gluLookAt(0.5, my_camera.getzoomdistance()*-sin(my_camera.getviewdirection())+0.5, my_camera.getzoomdistance()*cos(my_camera.getviewdirection())+0.5, 0.5, 0.5, 0.5, 0.0, cos(my_camera.getviewdirection()), sin(my_camera.getviewdirection()));

	//gluLookAt(0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0);
}

void drawSidepanel()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.2, 0.2, 0.2);
	//Dark gray background
	glRectf(1,0,ratio,1);
	glColor3f(1.0, 1.0, 1.0);
	writetext(1.02, .9, .04, "Score: &e0");
	//Debugging output, remove in release
	writetext(1.02, .5, .02, strprintf("Health: %g", p.health));
	writetext(1.02, .5-.02*textlineheight/textheight, .02, strprintf("Level: %d", p.level));
	//Debugging output, but everyone loves FPS counters, so it'll probably stay
	writetext(1.02, .02, .03, fps);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//A square viewport on the left side of the window
	glViewport(widthoffset, heightoffset, stagewidth, stagewidth);

	//Blank out unused portions of the screen
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0,0,0);
	glRectf(0,0,1,1);

	positionCamera();
	//Draw the scene
	drawBackground();
	p.draw();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&entity::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&entity::draw));

	//The sidepanel's viewport
	glViewport(widthoffset+stagewidth, heightoffset, sidewidth, stagewidth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(1, ratio, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawSidepanel();

	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (w==0 || h==0)
		return;
	int viewh, vieww;
	//Ensure the relative dimensions of the playing area are unstretched
	if (ratio*h>w)
	{
		vieww=w;
		viewh=w/ratio;
	}
	else
	{
		vieww=h*ratio;
		viewh=h;
	}
	//Center the viewing window
	widthoffset=(w-vieww)/2;
	heightoffset=(h-viewh)/2;
	stagewidth=viewh;
	sidewidth=vieww-viewh;
}

void stepandcull(vector<entity> &v)
{
	for_each(v.begin(), v.end(), mem_fun_ref(&entity::step));
	auto i=remove_if(v.begin(), v.end(), mem_fun_ref(&entity::isdestroyed));
	v.erase(i, v.end());
}

void spawnbullets(int tick, unordered_multimap<int, entity> &e)
{
	auto ents=e.equal_range(tick);
	for_each(ents.first, ents.second, [](const pair<int, entity> &p){spawnbullet(p.second);});
	e.erase(ents.first, ents.second);
}

void spawnenemies(int tick, unordered_multimap<int, entity> &e)
{
	auto ents=e.equal_range(tick);
	for_each(ents.first, ents.second, [](const pair<int, entity> &p){spawnenemy(p.second);});
	e.erase(ents.first, ents.second);
}

void gamelogic(int)
{
	//Reset the timer at the beginning of the function so we minimize lag
	//if the game logic takes a while
	glutTimerFunc(15, gamelogic, 0);
	//There's got to be a better way to do this
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
	//Move the player's bullets
	stepandcull(pb);
	//Move the enemies
	stepandcull(e);
	p.step(ticker);
	spawnbullets(ticker, lb);
	spawnenemies(ticker, le);
	++ticker;
	//This tracks ticks, not displayed frames (but it'll always be the
	//correct unless we take more than 16 milliseconds per tick)
	if (ticker%32==0)
	{
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		double diff=(now.tv_sec-then.tv_sec)+(now.tv_nsec-then.tv_nsec)/1000000000.0;
		double frames=32/diff;
		fps=strprintf("FPS: %05.2f", frames);
		then=now; //This happens soon
	}
	glutPostRedisplay();
}

//For the keyboard functions, we're relying on all keys starting in the up
//state and never going above the up state or below the down state.  I
//guarantee this will happen at least once in testing, even though it's
//logically impossible.
void keydown(unsigned char key, int, int)
{
}

void keyup(unsigned char key, int, int)
{
}

void specialdown(int key, int, int)
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

void specialup(int key, int, int)
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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	if (argc==1)
	{
		cout << "Usage: " << argv[0] << " datafile\n";
		return 1;
	}
	try
	{
		parse(argv[1]);
	}
	catch (string s)
	{
		cout << s << endl;
		return 2;
	}
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Inaequalem");

	//Something something POSIX
	clock_gettime(CLOCK_MONOTONIC, &then);

	glClearColor(1,1,1,0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	background[0]=loadRGBtexture("background.dat", 512, 2048);
	background[1]=loadRGBAtexture("foreground.dat", 512, 2048);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutTimerFunc(15, gamelogic, 0);
	glutIgnoreKeyRepeat(true);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(specialdown);
	glutSpecialUpFunc(specialup);


	glutMainLoop();
}
