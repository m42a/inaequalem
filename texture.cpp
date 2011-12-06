#include <fstream>
#include <memory>

#include "glut.h"
#include "texture.h"

using namespace std;

GLuint loadRGBtexture(const std::string &file, int width, int height)
{
	int size=3*width*height;
	unique_ptr<char> data(new char[size]);

	ifstream f(file);
	int n=0;
	while (n!=size && f)
	{
		f.read(data.get()+n, size-n);
		n+=f.gcount();
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Replace the host rectangle's properties with the texture's
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//Interpolate in the nicest way
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Wrap the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get());

	return texture;
}

GLuint loadRGBAtexture(const std::string &file, int width, int height)
{
	int size=4*width*height;
	unique_ptr<char> data(new char[size]);

	ifstream f(file);
	int n=0;
	while (n!=size && f)
	{
		f.read(data.get()+n, size-n);
		n+=f.gcount();
	}

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	//Replace the host rectangle's properties with the texture's
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//Interpolate in the nicest way
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Wrap the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.get());

	return texture;
}
