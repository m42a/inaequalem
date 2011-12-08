#ifndef AI_H_
#define AI_H_

#include <istream>
#include <memory>

#include "damage.h"
#include "glut.h"

//Don't include entity.h because it includes us
class entity;

//Base class; try not to instantiate it
class ai
{
public:
	//Do we even need this?  And why is it not const?  It's not like the ai can change type.
	int type;
	//The host body
	entity *body;

	ai(int t=0, entity *e=nullptr) : type(t), body(e) {}
	ai(const ai &, entity *e=nullptr) : type(0), body(e) {}

	//Required functions

	//Parses a new ai; should return the derived type
	static std::unique_ptr<ai> parse(std::istream &) {return std::unique_ptr<ai>(new ai());}
	//Called once every frame (roughly)
	virtual void step() {}
	//Clones the current ai, but changes the parent entity
	virtual std::unique_ptr<ai> clone(entity *e) const {return std::unique_ptr<ai>(new ai(*this, e));}
	virtual ~ai() {}

	//Optional functions
	virtual void takedamage(damage type, float dam);
};

//Every subclass of ai must override every required function, and may override
//optional functions.

//Remember that calling body->destroy() will delete your instantiation, and
//should be handled with care.

//This accelerates by a certain amount every frame
class newtonian : public ai
{
public:
	float velx, vely;
	float accx, accy;

	newtonian() : ai(1), velx(0), vely(0), accx(0), accy(0) {}
	newtonian(float vx, float vy, float ax, float ay, entity *e=nullptr) : ai(1,e), velx(vx), vely(vy), accx(ax), accy(ay) {}
	newtonian(const newtonian &n, entity *e=nullptr) : ai(1,e), velx(n.velx), vely(n.vely), accx(n.accx), accy(n.accy) {}

	static std::unique_ptr<ai> parse(std::istream &in);
	virtual void step();
	virtual std::unique_ptr<ai> clone(entity *e) const {return std::unique_ptr<ai>(new newtonian(*this, e));}
	virtual ~newtonian() {}
};

#endif
