#include "enemy.h"

using namespace std;

void enemy::step()
{
	if (isdestroyed())
		return;
	++counter;
}

void enemy::draw() const
{
}

void enemy::damage(float d)
{
	health-=d;
	//This should support death animations and be AI specific
	if (health<=0)
		destroy();
}
