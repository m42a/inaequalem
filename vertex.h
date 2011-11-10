#ifndef VERTEX_H_
#define VERTEX_H_

//This is pretty much just for passing to glVertex3fv
class vertex
{
public:
	float p[3];

	vertex(float x=0, float y=0, float z=0) : p({x,y,z}) {}
	operator float*() {return p;}
	operator const float*() const {return p;}
};

#endif
