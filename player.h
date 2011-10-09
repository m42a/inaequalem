#ifndef PLAYER_H_
#define PLAYER_H_

#include "inaequalem.h"

const float SQRT_3=1.73205080756887729;

class player
{
public:
	player(float xx, float yy, float s=.01) : x(xx), y(yy), speed(s) {}
	void draw() const;

	void move(float angle);
	void moveto(float, float);

	float x, y;
	float speed;
};

#endif
