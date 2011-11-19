#ifndef PLAYER_H_
#define PLAYER_H_

#include <cfloat>
#include <cmath>
#include <string>

#include "glut.h"
#include "model.h"

//#include "inaequalem.h"
//#include "entity.h"

class player
{
public:
	player(float xx, float yy, float s=.01) : x(xx), y(yy), speed(s), health(FLT_MIN), m("player") {}

	void step();
	void draw() const;

	void moveto(float xx, float yy) {x=xx; y=yy;}
	void moveby(float xx, float yy) {x+=xx; y+=yy;}
	void moveat(float speed, float dir) {x+=speed*cos(dir); y+=speed*sin(dir);}
	void move(float dir) {moveat(speed, dir);}

	float x, y;
	float speed;
	float health;
	const std::string m;
};

#endif
