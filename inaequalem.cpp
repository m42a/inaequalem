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
int movedir=0;
int ticker=0;
int background[2];
int camerarot=0;
int camerazoom=0;

int widthoffset;
int heightoffset;
int stagewidth;
int sidewidth;

struct timespec then;
string fps; //This is the string to display as an FPS counter, do not rely on it being parsable

//This is us and our bullets
player p(.5, .2);
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

void initparams()
{
	glClearColor(1,1,1,0);

	//Antialiasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//Texture loading
	background[0]=loadRGBtexture("background.dat", 512, 2048);
	background[1]=loadRGBAtexture("foreground.dat", 512, 2048);

	float none[]={0,0,0,0};
	float ambient[]={0,.1,0,1};
	float diffuse[]={1,1,1,1};

	//Light 0 is for the whole scene, light 1 is for the portals.  They
	//both come from the player.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, none);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, none);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
	//glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, none);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 10.0);
	glEnable(GL_LIGHT1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient);

	glShadeModel(GL_SMOOTH);
}

void drawBackground()
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	//Offset the background slightly so it doesn't get clipped
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, ticker/1200.0);
		glVertex3f(0,0,0.0001);
		glTexCoord2f(1, ticker/1200.0);
		glVertex3f(1,0,0.0001);
		glTexCoord2f(1, .25+ticker/1200.0);
		glVertex3f(1,1,0.0001);
		glTexCoord2f(0, .25+ticker/1200.0);
		glVertex3f(0,1,0);
	}
	glEnd();
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, background[1]);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, ticker/1000.0);
		glVertex3f(0,0,0.0002);
		glTexCoord2f(1, ticker/1000.0);
		glVertex3f(1,0,0.0002);
		glTexCoord2f(1, .25+ticker/1000.0);
		glVertex3f(1,1,0.0002);
		glTexCoord2f(0, .25+ticker/1000.0);
		glVertex3f(0,1,0.0002);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void positionCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Adjust this by zoom distance
	gluPerspective(60, 1, .1, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Look at the player
	my_camera.lookat(p.x, p.y, levelheight[p.level]);

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
	writetext(1.02, .5, .02, strprintf("Player x: %g", p.x));
	writetext(1.02, .5-.02*textlineheight/textheight, .02, strprintf("Player y: %g",p.y));
	writetext(1.02, .5-.04*textlineheight/textheight, .02, strprintf("Level: %d", p.level));
	float offset=(((0.8*p.x-1.6)*p.x+1.2)*p.x-0.4)*p.x-0.00128;
	writetext(1.02, .5-.06*textlineheight/textheight, .02, strprintf("Offset: %g", offset));
	//Debugging output, but everyone loves FPS counters, so it'll probably stay
	writetext(1.02, .02, .03, fps);
}

inline void enableClipping()
{
	//We need to redo this every time because this will change with the
	//modelview matrix.
	//Clipping planes are positioned such that if v0*x+v1*y+v2*z+v3<0, that vertex is clipped
	double planes[6][4]={
		{1, 0, 0, 0},
		{-1, 0, 0, 1},
		{0, 1, 0, 0},
		{0, -1, 0, 1},
		{0, 0, 1, 0},
		{0, 0, -1, 1}};
	//We're guaranteed at least 6 clipping planes
	for (int i=0; i<6; ++i)
	{
		//It is always the case that GL_CLIP_PLANEi==GL_CLIP_PLANE0+i.
		glClipPlane(GL_CLIP_PLANE0+i, planes[i]);
		glEnable(GL_CLIP_PLANE0+i);
	}
}

inline void disableClipping()
{
	for (int i=0; i<6; ++i)
		glDisable(GL_CLIP_PLANE0+i);
}

inline void enableLighting()
{
	float pos[]={p.x,p.y,levelheight[p.level],1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
}

inline void disableLighting()
{
	glDisable(GL_LIGHTING);
}

void drawback()
{
	if (p.y<.3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
		glTranslatef(p.x, -.505+.0075*(1-p.y), levelheight[p.level]);
		glRotatef(90,1,0,0);
		glColor4f(0,1,0,7*(.3-p.y)*(.3-p.y));
		glutSolidSphere(.5, 30, 30);
		glPopMatrix();
	}
}

void drawleftportal(float z)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	//float offset=-.05+(.8*(p.x-.5)*(p.x-.5)*(p.x-.5)*(p.x-.5)-.8/625.0);
	float offset=(((0.8*p.x-1.6)*p.x+1.2)*p.x-0.4)*p.x-0.00128;
	float opacity=(4.76*p.x-4.76)*p.x+0.9996;
	glTranslatef(offset, p.y, z);
	glColor4f(0,0,1,opacity);
	glRotatef(90,0,1,0);
	float height=.5*(.5-p.x)*(.5-p.x);
	glutSolidTorus(.05,height,10,10);
	glPopMatrix();
}

void drawrightportal(float z)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	float offset=(((0.8*p.x-1.6)*p.x+1.2)*p.x-0.4)*p.x-0.00128;
	glTranslatef(1-offset, p.y, z);
	float opacity=p.x*(4.76*p.x-4.76)+0.9996;
	glColor4f(1,.5,0,opacity);
	glRotatef(90,0,1,0);
	float height=.5*(.5-p.x)*(.5-p.x);
	glutSolidTorus(.05,height,10,10);
	glPopMatrix();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	//Resets the projection matrix
	positionCamera();
	//Draw the scene
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	//The player is magic, don't clip or depth-test them
	p.draw();
	glDepthFunc(GL_LESS);
	enableClipping();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&entity::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&entity::draw));
	drawBackground();
	if (p.x<.3)
		drawleftportal(levelheight[p.level]);
	if (p.x>.7)
		drawrightportal(levelheight[p.level]);
	drawback();
	disableClipping();

	glPushMatrix();
	glTranslatef(1,0,-distlevelup(p.level));
	enableClipping();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&entity::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&entity::draw));
	drawBackground();
	if (p.x>.7)
		drawleftportal(levelheight[(p.level+1)%ARRAYSIZE(levelheight)]);
	disableClipping();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1,0,-distleveldown(p.level));
	enableClipping();
	for_each(e.cbegin(), e.cend(), mem_fun_ref(&entity::draw));
	for_each(pb.cbegin(), pb.cend(), mem_fun_ref(&entity::draw));
	drawBackground();
	if (p.x<.3)
		drawrightportal(levelheight[(p.level-1+ARRAYSIZE(levelheight))%ARRAYSIZE(levelheight)]);
	disableClipping();
	glPopMatrix();

	glDepthFunc(GL_ALWAYS);
	//occludesides();
	glDisable(GL_DEPTH_TEST);

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
	if (camerarot==1)
		my_camera.incrementviewdirection();
	else if (camerarot==-1)
		my_camera.decrementviewdirection();
	if (camerazoom==1)
		my_camera.incrementzoomdistance();
	else if (camerazoom==-1)
		my_camera.decrementzoomdistance();
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
	if (key == 'q')
		camerarot+=1;
	if (key == 'a')
		camerarot-=1;
	if (key == 'w')
		camerazoom-=1;
	if (key == 's')
		camerazoom+=1;
}

void keyup(unsigned char key, int, int)
{
	if (key == 'q')
		camerarot-=1;
	if (key == 'a')
		camerarot+=1;
	if (key == 'w')
		camerazoom+=1;
	if (key == 's')
		camerazoom-=1;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Inaequalem");

	//Something something POSIX
	clock_gettime(CLOCK_MONOTONIC, &then);

	initparams();

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
