#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "color.h"
#include "vertex.h"
#include "coloredvertex.h"

inline float xnormal(const vertex & v0, const vertex & v1, const vertex & v2)
{
	return v1.p[2]*(v0.p[1] - v2.p[1]) + v0.p[2]*(v2.p[1] - v1.p[1]) + (v1.p[1] - v0.p[1])*v2.p[2];
}

inline float ynormal(const vertex & v0, const vertex & v1, const vertex & v2)
{
	return v0.p[2]*(v1.p[0] - v2.p[0]) + v1.p[2]*(v2.p[0] - v0.p[0]) + (v0.p[0] - v1.p[0])*v2.p[2];
}

inline float znormal(const vertex & v0, const vertex & v1, const vertex & v2)
{
	return v1.p[1]*(v0.p[0] - v2.p[0]) + v0.p[1]*(v2.p[0] - v1.p[0]) + (v1.p[0] - v0.p[0])*v2.p[1];
}

struct triangle
{
	coloredvertex v[3];
	float n[3];

	triangle() = default;
	triangle(const coloredvertex &v0, const coloredvertex &v1, const coloredvertex &v2) : n({xnormal(v0.v,v1.v,v2.v),ynormal(v0.v,v1.v,v2.v),znormal(v0.v,v1.v,v2.v)}) {v[0]=v0;v[1]=v1;v[2]=v2;}

	void draw() const {glNormal3fv(n);v[0].draw();v[1].draw();v[2].draw();}
};

#endif
