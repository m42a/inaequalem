#ifndef BULLET_H_
#define BULLET_H_

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

class bullet
{
public:
	void step();
	void destroy() {counter=-1;}
	bool isdestroyed() {return counter==-1;}
private:
	int counter;
	int ai;
	float direction;
	float speed;

	float x, y;
};

inline void step(bullet &b) {b.step();}

#endif
