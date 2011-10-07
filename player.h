#ifndef PLAYER_H_
#define PLAYER_H_

#include "inaequalem.h"

const float SQRT_3=1.73205080756887729;

class player
{
public:
	player(float xx, float yy) : x(xx), y(yy), speed(.01) {}
	void draw() const;

	void move(float angle);
	void moveto(float, float);

	static const float right=0*M_PI/4;
	static const float upright=1*M_PI/4;
	static const float up=2*M_PI/4;
	static const float upleft=3*M_PI/4;
	static const float left=4*M_PI/4;
	static const float downleft=5*M_PI/4;
	static const float down=6*M_PI/4;
	static const float downright=7*M_PI/4;
private:
	float x, y;
	float speed;
};

#endif
