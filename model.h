#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "coloredvertex.h"

class model
{
public:
	std::vector<coloredvertex> triangles;

	void addvertex(const vertex &v, const color &c=color::nocolor()) {triangles.emplace_back(v, c);}
	void addvertex(const coloredvertex &v) {triangles.push_back(v);}

	void draw() const {std::for_each(triangles.cbegin(), triangles.cend(), std::mem_fun_ref(&coloredvertex::draw));}

	//static std::unordered_map models;
};

#endif
