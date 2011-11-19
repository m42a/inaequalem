#ifndef INAEQUALEM_H_
#define INAEQUALEM_H_

#include <string>
#include <cmath>
#include <cfloat>
#include <vector>

#include "player.h"
#include "entity.h"
#include "vertex.h"
#include "glut.h"

//constants
extern player p;
extern std::vector<entity> pb;
extern std::vector<entity> e;

//Degrees counter-clockwise from straight right; this is fairly standard.
//These should be used as base values and not added together because that would
//be confusing.
//sprintf, but better
template <class... args>
std::string strprintf(const std::string &format, args... a)
{
		int size=snprintf(NULL, 0, format.c_str(), a...);
		//Screw it, this code only works with gcc anyway
		char s[size+1];
		sprintf(s, format.c_str(), a...);
		return s;
}

//I don't think this is used, but it might one day be useful
template <class T>
inline T bound(const T &v, const T &l, const T &r)
{
	if (v<l)
		return l;
	if (v>r)
		return r;
	return v;
}

inline entity &spawnenemy(float x, float y, const std::unique_ptr<ai> &a, const std::string m="", float h=1)
{
	e.emplace_back(x,y,a,m,h);
	return e.back();
}

inline entity &spawnbullet(float x, float y, const std::unique_ptr<ai> &a, const std::string m="", float h=FLT_MIN)
{
	pb.emplace_back(x,y,a,m,h);
	return pb.back();
}

#endif
