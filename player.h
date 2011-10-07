#ifndef PLAYER_H_
#define PLAYER_H_

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define SQRT_3 1.73205080756887729

class player
{
public:
	player(float xx, float yy) : x(xx), y(yy) {}
	void draw() const;
private:
	float x, y;
};

#endif
