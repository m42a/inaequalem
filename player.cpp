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
