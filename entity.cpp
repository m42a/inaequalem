#include "entity.h"

using namespace std;

/*void entity::step()
{
	if (isdestroyed())
		return;
	pai->step(this);
}*/

//This should be abstracted into the model class, instead of every entity being
//a cyan pentagon
void entity::draw() const
{
	//glTranslatef(x,y,0);
	//model::models[m].draw();
	//glTranslatef(-x,-y,0);
	float direction=0;
	glColor3f(0,1,1);
	drawaapolygon({
			{x+.01f*cos(direction),y+.01f*sin(direction)},
			{x+.01f*cos(direction+M_TAU/5),y+.01f*sin(direction+M_TAU/5)},
			{x+.01f*cos(direction+2*M_TAU/5),y+.01f*sin(direction+2*M_TAU/5)},
			{x+.01f*cos(direction+3*M_TAU/5),y+.01f*sin(direction+3*M_TAU/5)},
			{x+.01f*cos(direction+4*M_TAU/5),y+.01f*sin(direction+4*M_TAU/5)}});
}
