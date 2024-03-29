#include <memory>

#include "player.h"
#include "ai.h"
#include "inaequalem.h"

using namespace std;

const unique_ptr<ai> bulletai(new newtonian(0, .02, 0, 0));

void player::step(int tick)
{
	if (tick%4==0)
		spawnbullet({x, y+.09f, bulletai, "bullet", FLT_MIN, level});
}

void player::moveto(float xx, float yy)
{
	y=bound(yy, 0.03f, 0.9f);
	x=xx;
	while (x<0)
	{
		moveleveldown();
		x+=1;
	}
	while (x>1)
	{
		movelevelup();
		x-=1;
	}
}

void player::draw() const
{
	glPushMatrix();
	glTranslatef(x,y,levelheight[level]);
	models[m].draw();
	glPopMatrix();
}

