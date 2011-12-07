#include <cstdlib>

#include "glut.h"
#include "inaequalem.h"

using namespace std;

class camera
{
public:
	camera();
	static constexpr float xzoom = 0.1;
	static constexpr float zoomin = 0.1;
	void incrementviewdirection();
	void decrementviewdirection();
	//void zoomall();
	void incrementzoomdistance() { zoomdistance += xzoom;}
	void decrementzoomdistance();
	void lookat(float x, float y, float z);
	float getviewdirection() const {return viewdirection;}
	float getzoomdistance() const {return zoomdistance;}
private:
	float viewdirection;
	float zoomdistance;
};
camera::camera() : viewdirection(M_TAU/5), zoomdistance(1.0)
{
	/*viewdirection = 0.0;
	zoomdistance = 0.001; //stock values, might need changing.*/
}

void camera::decrementzoomdistance()
{
	zoomdistance-=xzoom;
	if (zoomdistance<0)
		zoomdistance=0;
}

void camera::incrementviewdirection()
{
	viewdirection += zoomin;
	if (viewdirection > M_TAU) //making circles
		viewdirection -= M_TAU;
}

void camera::decrementviewdirection()
{
	viewdirection -= zoomin;
	if(viewdirection < 0.0)
		viewdirection +=M_TAU;
}

void camera::lookat(float x, float y, float z)
{
	gluLookAt(
		x, y-getzoomdistance()*sin(getviewdirection()), z+getzoomdistance()*cos(getviewdirection()),
		x, y, z,
		0.0, cos(getviewdirection()), sin(getviewdirection()));
}
