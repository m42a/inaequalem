#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cfloat>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc!=4)
		return 1;
	char * infile=argv[1];
	char * outfile=argv[2];
	float size=atof(argv[3]);
	float offx=0, offy=0, offz=0;
	float maxx=-FLT_MAX, minx=FLT_MAX, maxy=-FLT_MAX, miny=FLT_MAX, maxz=-FLT_MAX, minz=FLT_MAX;
	vector<string> v;
	ifstream ifs(infile);
	string s;
	int numverts=0;
	while (getline(ifs, s))
	{
		v.push_back(s);
		if (s[0]=='v')
		{
			++numverts;
			istringstream istr(s);
			char c;
			float x,y,z;
			istr >> c >> x >> y >> z;
			if (x<minx)
				minx=x;
			if (x>maxx)
				maxx=x;
			if (y<miny)
				miny=y;
			if (y>maxy)
				maxy=y;
			if (z<minz)
				minz=z;
			if (z>maxz)
				maxz=z;
			offx+=x;
			offy+=y;
			offz+=z;
		}
	}
	cout << offx << ' ' << offy << ' ' << offz << '\n';
	offx/=numverts;
	offy/=numverts;
	offz/=numverts;
	cout << numverts << '\n';
	cout << offx << ' ' << offy << ' ' << offz << '\n';
	/*minx+=offx;
	maxx+=offx;
	miny+=offy;
	maxy+=offy;
	minz+=offz;
	maxz+=offz;*/
	float xdiff=maxx-minx;
	float ydiff=maxy-miny;
	float zdiff=maxz-minz;
	cout << xdiff << ' ' << ydiff << ' ' << zdiff << '\n';
	float maxdiff=xdiff;
	if (ydiff>maxdiff)
		maxdiff=ydiff;
	if (zdiff>maxdiff)
		maxdiff=zdiff;
	float scale=size/maxdiff;
	ofstream ofs(outfile);
	for (unsigned int i=0; i<v.size(); ++i)
	{
		if (v[i][0]=='v')
		{
			istringstream istr(v[i]);
			char c;
			float x,y,z;
			istr >> c >> x >> y >> z;
			ofs << "v " << (x-offx)*scale << ' ' << (y-offy)*scale << ' ' << (z-offz)*scale << '\n';
		}
		else
			ofs << v[i] << '\n';
	}
}

