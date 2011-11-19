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

	ai() : type(0), body(nullptr) {}

	ai(const ai &, entity *e=nullptr) : type(0), body(e) {}

	//Required functions
	//virtual ai &operator=(const ai &);
	static ai parse(std::istream &) {return ai();}
	virtual void step() {}
	virtual std::unique_ptr<ai> clone(entity *) {return std::unique_ptr<ai>(new ai(*this));}
	virtual ~ai();

	//Optional functions
	virtual void takedamage(damage type, float dam);
	//virtual void collide(
};

//Every subclass of ai must override every required function, and may override
//optional functions.

#endif
