#ifndef INAEQUALEM_H_
#define INAEQUALEM_H_

#include <algorithm>
#include <initializer_list>
#include <string>
#include <cmath>
#include "vertex.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

//constants
const float M_2PI=2*M_PI;

namespace direction
{
	const float right=0*M_2PI/8;
	const float upright=1*M_2PI/8;
	const float up=2*M_2PI/8;
	const float upleft=3*M_2PI/8;
	const float left=4*M_2PI/8;
	const float downleft=5*M_2PI/8;
	const float down=6*M_2PI/8;
	const float downright=7*M_2PI/8;
};

template <class... args>
std::string strprintf(const std::string &format, args... a)
{
		int size=snprintf(NULL, 0, format.c_str(), a...);
		//Screw it, this code only works with gcc anyway
		char s[size+1];
		sprintf(s, format.c_str(), a...);
		return s;
}

template <class T>
inline T bound(const T &v, const T &l, const T &r)
{
	if (v<l)
		return l;
	if (v>r)
		return r;
	return v;
}

//This is a hack to work around the fact that polygon smoothing doesn't do anything
template <class T>
void drawaapolygoniter(T b, T e)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	std::for_each(b, e, glVertex3fv);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	std::for_each(b, e, glVertex3fv);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//This is a hack to make the first hack work with initializer_lists, since they can't be deduced from templates
inline void drawaapolygon(std::initializer_list<vertex> il)
{
	drawaapolygoniter(il.begin(), il.end());
}

template <class T>
inline void drawaapolygon(T t)
{
	drawaapolygoniter(t.cbegin(), t.cend());
}

#endif
