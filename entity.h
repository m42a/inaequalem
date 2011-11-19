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

class entity
{
public:
	entity(float xx, float yy, ai *a=nullptr, std::string mm="") : x(xx), y(yy), pai(a), m(mm) {}
	entity(const entity &e) : x(e.x), y(e.y), pai(new ai(*e.pai)), m(e.m) {}
	entity &operator=(const entity &e) {moveto(e.x,e.y); m=e.m; pai=std::unique_ptr<ai>(new ai(*e.pai)); return *this;}

	void step() {if (!isdestroyed()) pai->step(this);}
	void draw() const;

	bool isdestroyed() const {return !pai.get();}
	bool hasmodel() const {return ismodel(m);}

	void destroy() {pai=std::unique_ptr<ai>(nullptr);}
	void moveto(float xx, float yy) {x=xx; y=yy;}
	void moveby(float xx, float yy) {x+=xx; y+=yy;}
	void moveat(float speed, float dir) {x+=speed*cos(dir); y+=speed*sin(dir);}

	float x, y;
	std::unique_ptr<ai> pai;
	std::string m;
};

#endif
