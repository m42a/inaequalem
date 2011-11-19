#include <cfloat>

#include "ai.h"
#include "entity.h"

//ai &ai::operator=(const ai &) = default;

ai::~ai() = default;

void ai::takedamage(damage type, float dam)
{
	if (type==damage::none && dam!=0)
		body->health-=FLT_MIN;
	else
		body->health-=dam;
}

