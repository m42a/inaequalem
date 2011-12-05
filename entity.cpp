#include <cfloat>

#include "entity.h"

using namespace std;

void entity::draw() const
{
	glPushMatrix();
	glTranslatef(x,y,levelheight[level]);
	models[m].draw();
	glPopMatrix();
}

//This destructor is implicitly huge, so don't inline it.
entity::~entity()
{
}
