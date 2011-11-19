#ifndef ENTITY_H_
#define ENTITY_H_

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "glut.h"
#include "ai.h"
#include "model.h"
#include "damage.h"

struct entity
{
	entity(float xx, float yy, ai *a=nullptr, std::string mm="", float h=0) : x(xx), y(yy), pai(a), m(mm), health(h) {}
	entity(const entity &e) : x(e.x), y(e.y), pai(e.pai->clone(this)), m(e.m), health(e.health) {}
	entity &operator=(const entity &e) {moveto(e.x,e.y); m=e.m; pai=e.pai->clone(this); health=e.health; return *this;}

	void step() {if (!isdestroyed()) pai->step();}
	void draw() const;

	bool isdestroyed() const {return !pai.get();}
	bool hasmodel() const {return ismodel(m);}

	void destroy() {pai=std::unique_ptr<ai>(nullptr);}
	void takedamage(damage type, float dam) {pai->takedamage(type, dam);}
	void moveto(float xx, float yy) {x=xx; y=yy;}
	void moveby(float xx, float yy) {x+=xx; y+=yy;}
	void moveat(float speed, float dir) {x+=speed*cos(dir); y+=speed*sin(dir);}

	float x, y;
	std::unique_ptr<ai> pai;
	std::string m;
	float health;
};

#endif
