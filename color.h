#ifndef COLOR_H_
#define COLOR_H_

#include "glut.h"

//This is pretty much just for passing to glColor4fv
struct color
{
	float p[4];

	color() = default;
	color(float r, float g, float b, float a=1) : p({r,g,b,a}) {}
	operator float*() {return p;}
	operator const float*() const {return p;}

	void draw() const {if (p[3]!=-1) glColor4fv(p);}

	//This is a value that indicates the color should be not changed.  For
	//drawing an object without any color, set the opacity to 0.
	static const color nocolor() {return {0,0,0,-1};}
};

#endif
