#ifndef BULLET_H_
#define BULLET_H_

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <vector>

#include "inaequalem.h"

class bullet
{
public:
	bullet(float xx, float yy, int a=0, float d=0, float s=1) : counter(0), ai(a), direction(d), speed(s), x(xx), y(yy) {}
	void setpos(float xx, float yy) {x=xx;y=yy;}
	void setspeed(float s) {speed=s;}
	void setdirection(float d) {direction=d;}

	void setai(int a) {ai=a;}
	//void settype(int t) {type=t;}

	void step();
	void draw() const;
	void destroy() {counter=-1;}
	bool isdestroyed() const {return counter==-1;}
//private:
	int counter;
	int ai;
	float direction;
	float speed;

	float x, y;
};

//inline void step(bullet &b) {b.step();}

#endif
