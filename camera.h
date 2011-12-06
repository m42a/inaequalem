#include<cstdlib>
#include "glut.h"
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
		float getviewdirection() const{return viewdirection;}
		float getzoomdistance() const{return zoomdistance;}
	private:
		float viewdirection;
		float zoomdistance;
};
camera::camera()
{
	viewdirection =0.0;
	zoomdistance = 0.001; //stock values, might need changing.
}

void camera::decrementzoomdistance()
{	
	if((zoomdistance-xzoom) >= 0.0)
	{
		zoomdistance -= xzoom;
		return;
	}
	zoomdistance = 0.0;
	return;
}

void camera::incrementviewdirection()
{
	viewdirection += zoomin;
	if (viewdirection > 360.0) //making circles
		viewdirection -= 360.0;
}

void camera::decrementviewdirection()
{
	viewdirection -= zoomin;
	if(viewdirection < 0.0)
		viewdirection +=360.0;
}
