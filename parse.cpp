#include <fstream>
#include <string>
#include <unordered_map>

#include <iostream>

#include "parse.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"
#include "color.h"

using namespace std;

unordered_map<string, color> colors;
unordered_map<string, vertex> vertices;
unordered_map<string, triangle> triangles;

inline bool iscolor(string s)
{
	return colors.count(s);
}

inline bool isvertex(string s)
{
	return vertices.count(s);
}

inline bool istriangle(string s)
{
	return triangles.count(s);
}

inline void addcolor(string name, const color &c)
{
	colors[name]=c;
}

inline void addtriangle(string name, const triangle &t)
{
	triangles[name]=t;
}

inline void addvertex(string name, const vertex &v)
{
	vertices[name]=v;
}

string parsestring(istream &in)
{
	char c;
	//Discard whitespace
	while (in.get(c) && isspace(c))
		;
	if (in.eof())
		throw string("Error: unexpected end of file.");
	if (in.fail())
		throw string("An unexpected error occured while reading from the file.");
	string val;
	//We found a double-hyphen
	if (c=='-' && in.peek()=='-')
	{
		in.get();
		return "--";
	}
	//A type 1 string
	if (c!='"')
	{
		val=c;
		//This will pull the next whitespace character out of the
		//stream, but we never use those anyway.
		while (in.get(c) && !isspace(c))
			val.push_back(c);
		//Be tolerant and assume an EOF delimits a type 1 string.
		return val;
	}
	//A type 2 string
	while (in.get(c) && c!='"')
	{
		//Next character gets treated as a literal
		if (c=='\\')
			in.get(c);
		val.push_back(c);
	}
	if (c!='"')
		throw string("Error: unexpected end of string.");
	return val;
}

vertex parsevertex(istream &in)
{
	float f1, f2, f3;
	in >> f1 >> f2 >> f3;
	if (!in)
		throw string("Error parsing vertex.");
	return {f1,f2,f3};
}

color parsecolor(istream &in)
{
	float f1, f2, f3, f4;
	in >> f1 >> f2 >> f3 >> f4;
	if (!in)
		throw string("Error parsing color.");
	return {f1,f2,f3,f4};
}

coloredvertex parsecoloredvertex(istream &in)
{
	color c=color::nocolor();
	string name=parsestring(in);
	if (name=="color")
	{
		c=parsecolor(in);
		name=parsestring(in);
	}
	else if (iscolor(name))
	{
		c=colors[name];
		name=parsestring(in);
	}
	//Fall through after trying to parse the color, whether we succeed or not
	if (name=="vertex")
		return {parsevertex(in), c};
	if (isvertex(name))
		return {vertices[name], c};
	throw "Unexpected string \""+name+"\" encountered";
}

triangle parsetriangle(istream &in)
{
	coloredvertex cv1=parsecoloredvertex(in);
	coloredvertex cv2=parsecoloredvertex(in);
	coloredvertex cv3=parsecoloredvertex(in);
	return {cv1, cv2, cv3};
}

void donothing(istream &in)
{
	string tmp;
	while ((tmp=parsestring(in))!="--")
		;
}

void parsebullet(istream &in)
{
	donothing(in);
}

void parseenemy(istream &in)
{
	donothing(in);
}

void parsemodel(istream &in)
{
	string name=parsestring(in);
	if (!in || name=="--")
		throw string("Error: unnamed model");
	model m;
	string type;
	while ((type=parsestring(in))!="--" && in)
	{
		try
		{
			if (type=="triangle")
				m.addtriangle(parsetriangle(in));
			else if (istriangle(type))
				m.addtriangle(triangles[type]);
			else
				throw "Unrecognized triangle \""+type+"\".";
		}
		catch (string s)
		{
			throw "     In model \""+name+"\":\n"+s;
		}
	}
	models[name]=m;
	if (type!="--")
		throw string("Error: unexpected end of \"model\" directive");
}

void parsedefine(istream &in)
{
	string name;
	//in comes after the assignement since it modifies in
	while ((name=parsestring(in))!="--" && in)
	{
		try
		{
			string type=parsestring(in);
			if (type=="color")
				addcolor(name, parsecolor(in));
			else if (type=="vertex")
				addvertex(name, parsevertex(in));
			else if (type=="triangle")
				addtriangle(name, parsetriangle(in));
			else
				throw "Error: unrecognized type \""+type+"\".";
		}
		catch (string s)
		{
			throw "     In definition of \""+name+"\":\n"+s;
		}
	}
	//An error occured before the section ended
	if (name!="--")
		throw string("Error: unexpected end of \"define\" directive");
}

void parseinclude(istream &in)
{
	string fname;
	//in comes after the assignement since it modifies in
	while ((fname=parsestring(in))!="--" && in)
	{
		try
		{
			parse(fname);
		}
		catch (string s)
		{
			throw "   In file \""+fname+"\":\n"+s;
		}
	}
	//An error occured before the section ended
	if (fname!="--")
		throw string("Error: unexpected end of \"include\" directive");
}

//parse failing should be fatal
void parse(const string &fname)
{
	ifstream ifs(fname);
	if (!ifs.is_open())
		throw "Error: Couldn't open file \""+fname+"\"";
	string tmp;
	//Look for the next hyphen
	while (getline(ifs, tmp, '-'))
	{
		//Normally this would be peek, but if it's not a hyphen it
		//won't show up when we're looking for a hyphen, and if it is,
		//we'd just discard it to get to the identifier after it.
		if (ifs.get()!='-')
			continue;
		tmp=parsestring(ifs);
		if (tmp=="include")
			parseinclude(ifs);
		else if (tmp=="define")
			parsedefine(ifs);
		else if (tmp=="model")
			parsemodel(ifs);
		else if (tmp=="bullet")
			parsebullet(ifs);
		else if (tmp=="enemy")
			parseenemy(ifs);
		else
			throw "Error: \"" + tmp + "\" is not a valid identifier.";

	}
}
