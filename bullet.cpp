#include <cmath>

#include "bullet.h"

using namespace std;

void bullet::step()
{
	if (isdestroyed())
		return;
	//++counter;
	if (ai==0)
	{
		x+=speed*cos(direction);
		y+=speed*sin(direction);
	}
	if (x<-.1 || x>1.1 || y<-.1 || y>1.1)
		destroy();
}

void bullet::draw() const
{
	glColor3f(0,1,1);
	drawaapolygon({
			{x+.01f*cos(direction),y+.01f*sin(direction)},
			{x+.01f*cos(direction+M_TAU/5),y+.01f*sin(direction+M_TAU/5)},
			{x+.01f*cos(direction+2*M_TAU/5),y+.01f*sin(direction+2*M_TAU/5)},
			{x+.01f*cos(direction+3*M_TAU/5),y+.01f*sin(direction+3*M_TAU/5)},
			{x+.01f*cos(direction+4*M_TAU/5),y+.01f*sin(direction+4*M_TAU/5)}});
}
