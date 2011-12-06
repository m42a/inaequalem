#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

GLuint loadRGBtexture(const std::string &file, int width, int height);
GLuint loadRGBAtexture(const std::string &file, int width, int height);

#endif
