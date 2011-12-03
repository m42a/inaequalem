#ifndef AI_H_
#define AI_H_

#include <istream>
#include <memory>

#include "damage.h"
#include "glut.h"

class entity;

class ai
{
public:
	int type;
	entity *body;

	ai(int t=0, entity *e=nullptr) : type(t), body(e) {}

	ai(const ai &, entity *e=nullptr) : type(0), body(e) {}

	//Required functions
	//virtual ai &operator=(const ai &);
	static ai parse(std::istream &) {return ai();}
	virtual void step() {}
	virtual std::unique_ptr<ai> clone(entity *e) const {return std::unique_ptr<ai>(new ai(*this, e));}
	virtual ~ai() {}

	//Optional functions
	virtual void takedamage(damage type, float dam);
	//virtual void collide(
};

//Every subclass of ai must override every required function, and may override
//optional functions.

class newtonian : public ai //This accelerates by a certain amount every frame
{
public:
	float velx, vely;
	float accx, accy;

	newtonian() : ai(1), velx(0), vely(0), accx(0), accy(0) {}
	newtonian(float vx, float vy, float ax, float ay, entity *e=nullptr) : ai(1,e), velx(vx), vely(vy), accx(ax), accy(ay) {}
	newtonian(const newtonian &n, entity *e=nullptr) : ai(1,e), velx(n.velx), vely(n.vely), accx(n.accx), accy(n.accy) {}

	static newtonian parse(std::istream &in);
	virtual void step();
	virtual std::unique_ptr<ai> clone(entity *e) const {return std::unique_ptr<ai>(new newtonian(*this, e));}
	virtual ~newtonian() {}
};

#endif
