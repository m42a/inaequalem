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

//gcc added nullptr support in 4.6
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
#   define nullptr NULL
#endif

#define ARRAYSIZE(x) (sizeof(x)/sizeof(*(x)))

const float M_TAU=2*M_PI;

//Degrees counter-clockwise from straight right; this is fairly standard.
//These should be used as base values and not added together because that would
//be confusing.
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

//The height of each level; this should be strictly increasing unless you want
//to confuse people
const float levelheight[]={.2,.8};

#endif
