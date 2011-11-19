#ifndef GLUT_H_
#define GLUT_H_

#include <algorithm>
#include <initializer_list>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6) //gcc added nullptr support in 4.6
#   define nullptr NULL
#endif

class vertex;

const float M_TAU=2*M_PI;

namespace direction
{
	const float right=0*M_TAU/8;
	const float upright=1*M_TAU/8;
	const float up=2*M_TAU/8;
	const float upleft=3*M_TAU/8;
	const float left=4*M_TAU/8;
	const float downleft=5*M_TAU/8;
	const float down=6*M_TAU/8;
	const float downright=7*M_TAU/8;
};

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

//This is a hack to make the first hack work with initializer_lists, since they can't be deduced from templates (and don't use cbegin or cend)
inline void drawaapolygon(std::initializer_list<vertex> il)
{
	drawaapolygoniter(il.begin(), il.end());
}

template <class T>
inline void drawaapolygon(const T &t)
{
	drawaapolygoniter(t.cbegin(), t.cend());
}

template <class T>
void drawpolygoniter(T b, T e)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	std::for_each(b, e, glVertex3fv);
	glEnd();
}

//This is a hack to make the first hack work with initializer_lists, since they can't be deduced from templates (and don't use cbegin or cend)
inline void drawpolygon(std::initializer_list<vertex> il)
{
	drawpolygoniter(il.begin(), il.end());
}

template <class T>
inline void drawpolygon(const T &t)
{
	drawpolygoniter(t.cbegin(), t.cend());
}

#endif
