#include "enemy.h"

using namespace std;

void enemy::step()
{
	if (isdestroyed())
		return;
	++counter;
}

void enemy::damage(float d)
{
	health-=d;
	if (health<=0)
		destroy();
}
