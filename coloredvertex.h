#ifndef COLOREDVERTEX_H_
#define COLOREDVERTEX_H_

#include "vertex.h"
#include "color.h"

struct coloredvertex
{
	vertex v;
	color c;

	coloredvertex() : v({0,0,0}), c(color::nocolor()) {}
	coloredvertex(const vertex& vv, const color &cc=color::nocolor()) : v(vv), c(cc) {}

	void draw() const {c.draw(); v.draw();}
};

#endif
