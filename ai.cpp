#include <cfloat>

#include "ai.h"
#include "entity.h"

//ai &ai::operator=(const ai &) = default;

//ai::~ai() = default;

void ai::takedamage(damage type, float dam)
{
	if (type==damage::none && dam!=0)
		body->health-=FLT_MIN;
	else
		body->health-=dam;
	if (body->health<=0)
		body->destroy();
}

void newtonian::step()
{
	//if (!body)
	//	return;
	velx+=accx;
	vely+=accy;
	body->x+=velx;
	body->y+=vely;
	if (body->y<-.1 || body->y>1.1 || body->x<-.1 || body->x>1.1)
		body->destroy();
}
