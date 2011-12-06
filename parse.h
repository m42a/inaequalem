#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <typeinfo>
#include <sstream>
#include <istream>

void parse(const std::string &fname);
std::string parsestring(std::istream &in);

template <class T>
T parse(std::istream &in)
{
	std::istringstream istr(parsestring(in));
	T t;
	istr >> t;
	if (!istr)
		throw std::string("Error parsing ")+typeid(T).name()+std::string(" \"")+istr.str()+"\"";
	return t;
}

#endif
