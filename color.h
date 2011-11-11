#ifndef COLOR_H_
#define COLOR_H_

//This is pretty much just for passing to glColor4fv
class color
{
public:
	float p[4];

	vertex(float r, float g, float b, float a=1) : p({r,g,b,a}) {}
	operator float*() {return p;}
	operator const float*() const {return p;}
};

#endif
