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

using namespace std;

const float ratio=640.0/480.0; //The desired width/height ratio
const float textheight=119.05; //From the documentation
const float textlineheight=119.05+33.33; //From the documentation

// These should be part of the player class
int movedir=0;
//bool shooting;
int ticker=0;

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
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0,0,1,1);
}

void drawSidepanel()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.2, 0.2, 0.2);
	//Dark gray background
	glRectf(1,0,ratio,1);
	glColor3f(1.0, 1.0, 1.0);
	//Text in white
	writetext(1.02, .9, .04, "Score: &e0");
	//Debugging output, remove in release
	writetext(1.02, .5, .02, strprintf("pb.size()=%d",pb.size()));
	//Debugging output, remove in release
	if (pb.size()!=0)
		writetext(1.02, .5-.03*textlineheight/textheight, .02, strprintf("pb[0].y=%f",pb[0].y));
	//Debugging output, but everyone loves FPS counters, so it'll probably stay
	writetext(1.02, .02, .03, fps);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	p.draw();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&entity::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&entity::draw));
	//Draw the side panel over everything else, since we don't do scissoring
	drawSidepanel();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (w==0 || h==0)
		return;
	//Ensure the relative dimensions of the playing area are unstretched
	if (ratio*h>w)
		glViewport(0, (h-w/ratio)/2, w, w/ratio);
	else
		glViewport((w-h*ratio)/2, 0, h*ratio, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Change this when we have 3D
	gluOrtho2D(0,ratio,0,1);
	glMatrixMode(GL_MODELVIEW);
}

void stepandcull(vector<entity> &v)
{
	for_each(v.begin(), v.end(), mem_fun_ref(&entity::step));
	auto i=remove_if(v.begin(), v.end(), mem_fun_ref(&entity::isdestroyed));
	v.erase(i, v.end());
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
	auto aaa=lb.equal_range(ticker);
	for_each(aaa.first, aaa.second, [](const pair<int, entity> &p){spawnbullet(p.second);});
	lb.erase(aaa.first, aaa.second);
	aaa=le.equal_range(ticker);
	for_each(aaa.first, aaa.second, [](const pair<int, entity> &p){spawnenemy(p.second);});
	le.erase(aaa.first, aaa.second);
	++ticker;
	//Shoot every 4th tick
	/*if (ticker%4==0 && shooting)
		pb.emplace_back(p.x, p.y, 0, direction::up, .02);*/
	//This tracks ticks, not displayed frames (but it'll always be the
	//correct unless we take more than 16 milliseconds per tick)
	if (ticker%32==0)
	{
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		double diff=(now.tv_sec-then.tv_sec)+(now.tv_nsec-then.tv_nsec)/1000000000.0;
		double frames=32/diff;
		//This should really be a variadic template function
		//This is a function, there's no reason not to just call strprintf
		/*int size=snprintf(NULL, 0, "FPS: %05.2f", frames);
		char *s=new char[size+1];
		sprintf(s, "FPS: %05.2f", frames);
		fps=s;
		delete[] s;*/
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
	/*if (key=='z')
		shooting=true;*/
}

void keyup(unsigned char key, int, int)
{
	/*if (key=='z')
		shooting=false;*/
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
	//This doesn't even work, but it's okay, we have a stupid workaround
	/*glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);*/

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
