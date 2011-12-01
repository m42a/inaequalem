#include <memory>

#include "player.h"
#include "ai.h"
#include "inaequalem.h"

using namespace std;

const float SQRT_3=1.73205080756887729;

const unique_ptr<ai> bulletai(new newtonian(0, .02, 0, 0));

void player::step(int tick)
{
	if (tick%4==0)
		spawnbullet(x, y, bulletai, "bullet");
}

void player::draw() const
{
	glPushMatrix();
	glTranslatef(x,y,levelheight[level]);
	models[m].drawsolid();
	models[m].drawwireframe();
	glPopMatrix();
}

