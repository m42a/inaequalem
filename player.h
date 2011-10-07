#ifndef PLAYER_H_
#define PLAYER_H_

#include "inaequalem.h"

#define SQRT_3 1.73205080756887729

class player
{
public:
	player(float xx, float yy) : x(xx), y(yy), speed(.1) {}
	void draw() const;

	void move(float angle);
	void moveto(float, float);

	static const float right=0.0f;
	static const float up=M_PI/2;
	static const float left=M_PI;
	static const float down=3*M_PI/2;
private:
	float x, y;
	float speed;
};

#endif
