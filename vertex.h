#ifndef VERTEX_H_
#define VERTEX_H_

#include "glut.h"

//This is pretty much just for passing to glVertex3fv
struct vertex
{
	float p[3];

	vertex(float x=0, float y=0, float z=0) : p({x,y,z}) {}
	operator float*() {return p;}
	operator const float*() const {return p;}

	void draw() const {glVertex3fv(p);}
};

#endif
