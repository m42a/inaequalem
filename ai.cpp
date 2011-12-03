#include <cfloat>

#include "ai.h"
#include "entity.h"
#include "parse.h"

using namespace std;

void ai::takedamage(damage type, float dam)
{
	if (type==damage::none && dam!=0)
		body->health-=FLT_MIN;
	else
		body->health-=dam;
	if (body->health<=0)
		body->destroy();
}

newtonian newtonian::parse(istream &in)
{
	float velx=::parse<float>(in);
	float vely=::parse<float>(in);
	float accx=::parse<float>(in);
	float accy=::parse<float>(in);
	return newtonian(velx, vely, accx, accy);
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
