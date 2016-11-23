#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Vector4.h"
#include <fstream>

class Texture
{
public:
	Texture(std::string fileName, int width, int height); // Constructor, creates a texture from the path given as param(filename) with the width and height given.
	virtual ~Texture();
	ZyphMaths::Vector4 Map(float tu, float tv); // Map the tu and tv coordinates.

private:
	int m_width; // The width of the texture
	int m_height; // The height of the texture
	unsigned char* m_internalBuffer; // Buffer that holds the rgb values of the texture.
	void Load(std::string fileName); // Load in the texture with the path given as param.
};
#endif // !_TEXTURE_H_
