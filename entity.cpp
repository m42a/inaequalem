#include <cfloat>

#include "entity.h"

using namespace std;

void entity::draw() const
{
	glPushMatrix();
	glTranslatef(x,y,levelheight[level]);
	models[m].drawsolid();
	models[m].drawwireframe();
	glPopMatrix();
}

entity::~entity()
{
}
