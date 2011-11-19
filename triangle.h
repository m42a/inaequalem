#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "color.h"
#include "vertex.h"
#include "coloredvertex.h"

struct triangle
{
	coloredvertex v[3];

	triangle() = default;
	triangle(const coloredvertex &v0, const coloredvertex &v1, const coloredvertex &v2) /*: v({v1,v2,v3}) Stupid gcc bug */ {v[0]=v0;v[1]=v1;v[2]=v2;}

	void draw() const {v[0].draw();v[1].draw();v[2].draw();}
};

#endif
