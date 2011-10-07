#include "player.h"

using namespace std;

void player::draw() const
{
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(x, y+.1);
	glVertex2f(x+SQRT_3/20, y-.05);
	glVertex2f(x-SQRT_3/20, y-.05);
	glEnd();
}

void player::move(float angle)
{
	x=bound(x+speed*cos(angle),0.0f,1.0f);
	y=bound(y+speed*sin(angle),0.0f,1.0f);
}

void player::moveto(float xx, float yy)
{
	x=bound(xx,0.0f,1.0f);
	y=bound(yy,0.0f,1.0f);
}
