#ifndef ENEMY_H_
#define ENEMY_H_

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "bullet.h"

class enemy
{
public:
	void step();
	void destroy() {counter=-1;}
	bool isdestroyed() {return counter==-1;}
	void damage(float d);

	int counter;
	int type; //What sprite the enemy has
	int ai; //How the enemy moves

	float health;

	float x, y;
};

#endif
