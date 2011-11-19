#ifndef AI_H_
#define AI_H_

class entity;

class ai
{
private:

public:
	int type;

	ai() : type(0) {}
	ai(const ai &) = default;

	virtual ai &operator=(const ai &);

	virtual void step(entity *) {}

	virtual ~ai();
};

//Every subclass of ai must:
//Set type correctly
//Have a copy constructor
//Have a virtual operator=
//Have a virtual step function

#endif
