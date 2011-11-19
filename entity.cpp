#include <cfloat>

#include "entity.h"

using namespace std;

void entity::draw() const
{
	glTranslatef(x,y,0);
	models[m].drawsolid();
	glTranslatef(-x,-y,0);
}
