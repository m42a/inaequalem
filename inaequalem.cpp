#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <exception>

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
string fps; //This is the string to display as an FPS counter, do not rely on it being parsable

//This is us and our bullets
player p(.5, .5);
vector<bullet> pb;
//These are the enemies and their bullets
vector<enemy> e;
vector<bullet> eb;

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

void addtriangle(string name, const triangle &t)

void addvertex(string name, const vertex &v)

void addcolor(string name, const color &c)

vertex parsevertex(ifstream &ifs)
{
	float f1, f2, f3;
	ifs >> f1 >> f2 >> f3;
	if (!ifs)
		throw string("Error parsing vertex.");
	return {f1,f2,f3};
}

color parsecolor(ifstream &ifs)
{
	float f1, f2, f3, f4;
	ifs >> f1 >> f2 >> f3 >> f4;
	if (!ifs)
		throw string("Error parsing vertex.");
	return {f1,f2,f3,f4};
}

triangle parsetriangle(ifstream &ifs)

void parsebullet(ifstream &ifs)

void parseenemy(ifstream &ifs)

void parsemodel(ifstream &ifs)

void parsedefine(ifstream &ifs)
{
	string name;
	//ifs comes after the assignement sine it modifies ifs
	while ((name=parsestring(ifs))!="--" && ifs)
	{
		try
		{
			string type=parsestring(ifs);
			if (type=="color")
				addcolor(name, parsecolor(ifs));
			if (type=="vertex")
				addvertex(name, parsevertex(ifs));
			if (type=="triangle")
				addtriangle(name, parsetriangle(ifs));
			else
				throw "Error: unrecognized type \""+type+"\".";
		}
		catch (string s)
			throw "     In definition of \""+name+"\":\n"+s;
	}
	//An error occured before the section ended
	if (name!="--")
		throw string("Error: unexpected end of \"define\" directive");
}

void parseinclude(ifstream &ifs)
{
	string fname;
	//ifs comes after the assignement sine it modifies ifs
	while ((fname=parsestring(ifs))!="--" && ifs)
	{
		try
		{
			parse(fname);
		}
		catch (string s)
			throw "   In file \""+fname+"\":\n"+s;
	}
	//An error occured before the section ended
	if (fname!="--")
		throw string("Error: unexpected end of \"include\" directive");
}

string parsestring(ifstream &ifs)
{
	char c;
	//Discard whitespace
	while (ifs >> c && isspace(c))
		;
	if (ifs.eof())
		throw string("Error: unexpected end of file.");
	if (ifs.fail())
		throw string("An unexpected error occured while reading from the file.");
	string val;
	//We found a double-hyphen
	if (c=='-' && ifs.peek()=='-')
	{
		ifs >> c;
		return "--";
	}
	//A type 1 string
	if (c!='"')
	{
		val=c;
		//This will pull the next whitespace character out of the
		//stream, but we never use those anyway.
		while (ifs >> c && !isspace(c))
			val.push_back(c);
		//Be tolerant and assume an EOF delimits a type 1 string.
		return val;
	}
	//A type 2 string
	while (ifs >> c && c!='"')
	{
		//Next character gets treated as a literal
		if (c=='\\')
			ifs >> c;
		val.push_back(c);
	}
	if (c!='"')
		throw string("Error: unexpected end of string.");
	return val;
}

//parse failing should be fatal
void parse(const string &fname)
{
	ifstream ifs(fname);
	if (!ifs.is_open())
		throw "Error: file \""+fname+"\" does not exist";
	string tmp;
	//Look for the next hyphen
	while (getline(ifs, tmp, '-'))
	{
		//Normally this would be peek, but if it's not a hyphen it
		//won't show up when we're looking for a hyphen, and if it is,
		//we'd just discard it to get to the identifier after it.
		if (ifs.get()!='-')
			continue;
		tmp=parsestring(ifs);
		if (tmp=="include")
			parseinclude(ifs);
		else if (tmp=="define")
			parsedefine(ifs);
		else if (tmp=="model")
			parsemodel(ifs);
		else if (tmp=="bullet")
			parsebullet(ifs);
		else if (tmp=="enemy")
			parseenemy(ifs);
		else
			throw "Error: \"" + tmp + "\" is not a valid identifier.";

	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	p.draw();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&enemy::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&bullet::draw));
	for_each(eb.cbegin(), eb.cend(), mem_fun_ref(&bullet::draw));
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

template<class T>
void stepandcull(vector<T> &v)
{
	for_each(v.begin(), v.end(), mem_fun_ref(&T::step));
	auto i=remove_if(v.begin(), v.end(), mem_fun_ref(&T::isdestroyed));
	v.erase(i, v.end());
}

void gamelogic(int v)
{
	//Reset the timer at the beginning of the function so we minimize lag
	//if the game logic takes a while
	glutTimerFunc(16, gamelogic, 0);
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
	//Don't move the enemies or their bullets, since there's no collision detection
	++ticker;
	//Shoot every 4th tick
	if (ticker%4==0 && shooting)
		pb.emplace_back(p.x, p.y, 0, direction::up, .02);
	//This tracks ticks, not displayed frames (but it'll always be the
	//correct unless we take more than 16 milliseconds for a tick)
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

//I have a feeling we might not need this, but it's registered just in case
void mousemove(int x, int y)
{
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
