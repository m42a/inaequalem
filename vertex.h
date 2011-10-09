#ifndef VERTEX_H_
#define VERTEX_H_

class vertex
{
public:
	float p[3];

	vertex(float x=0, float y=0, float z=0) : p({x,y,z}) {}
	operator float*() {return p;}
	operator const float*() const {return p;}
};

#endif
