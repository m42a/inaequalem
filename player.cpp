#include "player.h"

using namespace std;

const float SQRT_3=1.73205080756887729;

void player::step()
{
}

void player::draw() const
{
	/*
	glColor3f(1,1,1);
	drawaapolygon({
			{x, y+.05f},
			{x+SQRT_3/40, y-.025f},
			{x-SQRT_3/40, y-.025f}});
	*/
	glTranslatef(x,y,0);
	models[m].drawsolid();
	models[m].drawwireframe();
	glTranslatef(-x,-y,0);
}

