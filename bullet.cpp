#include <cmath>

#include "bullet.h"

using namespace std;

void bullet::step()
{
	if (isdestroyed())
		return;
	++counter;
	if (ai==0)
	{
		x+=speed*cos(direction);
		y+=speed*sin(direction);
	}
	if (x<-.1 || x>1.1 || y<-.1 || y>1.1)
		destroy();
}

