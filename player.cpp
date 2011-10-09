#include "player.h"

using namespace std;

void player::draw() const
{
	glColor3f(1,1,1);
	drawaapolygon({
			{x, y+.05f},
			{x+SQRT_3/40, y-.025f},
			{x-SQRT_3/40, y-.025f}});
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex3f(x, y+.05, 0);
	glVertex3f(x+SQRT_3/40, y-.025, 0);
	glVertex3f(x-SQRT_3/40, y-.025, 0);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex3f(x, y+.05, 0);
	glVertex3f(x+SQRT_3/40, y-.025, 0);
	glVertex3f(x-SQRT_3/40, y-.025, 0);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
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
