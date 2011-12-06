#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>

#include "coloredvertex.h"
#include "triangle.h"

struct model
{
	std::vector<coloredvertex> triangles;

	void addvertex(const vertex &v, const color &c=color::nocolor()) {triangles.emplace_back(v, c);}
	void addvertex(const coloredvertex &v) {triangles.push_back(v);}
	void addtriangle(const triangle &t) {addvertex(t.v[0]);addvertex(t.v[1]);addvertex(t.v[2]);}
	void addmodel(const model &m) {triangles.insert(triangles.end(), m.triangles.cbegin(), m.triangles.cend());}

	void drawtriangles() const {glBegin(GL_TRIANGLES); std::for_each(triangles.cbegin(), triangles.cend(), std::mem_fun_ref(&coloredvertex::draw)); glEnd();}

	void drawsolid() const {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); drawtriangles();}
	void drawwireframe() const {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); drawtriangles();}
};

extern std::unordered_map<std::string, model> models;

inline bool ismodel(std::string s) {return models.count(s);}
inline void addmodel(std::string name, const model &m) {models[name]=m;}

#endif
