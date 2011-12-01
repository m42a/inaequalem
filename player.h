#ifndef PLAYER_H_
#define PLAYER_H_

#include <cfloat>
#include <cmath>
#include <string>

#include "glut.h"
#include "model.h"
#include "damage.h"

//#include "inaequalem.h"
//#include "entity.h"

class player
{
public:
	player(float xx, float yy, float s=.01, int l=0) : x(xx), y(yy), speed(s), health(FLT_MIN), m("player") {movelevel(l);}

	void step(int tick);
	void draw() const;

	void takedamage(damage type, float dam) {if (type!=damage::none) health-=dam;}
	void moveto(float xx, float yy) {x=xx; y=yy;}
	void moveby(float xx, float yy) {x+=xx; y+=yy;}
	void moveat(float speed, float dir) {x+=speed*cos(dir); y+=speed*sin(dir);}
	void move(float dir) {moveat(speed, dir);}
	void movelevel(int l) {level=l%ARRAYSIZE(levelheight); if(l<0) l+=ARRAYSIZE(levelheight);}
	void movelevelup() {movelevel(level+1);}
	void moveleveldown() {movelevel(level-1);}

	float x, y;
	int level;
	float speed;
	float health;
	const std::string m;
};

#endif
