#define _CRT_SECURE_NO_DEPRECATE
#include "texture.h"

#include <windows.h>


/// <summary>Creates a texture class from the filename given as param.</summary>
/// <seealso cref="Load(fileName)"/>
Texture::Texture(std::string fileName, int width, int height) :
	m_height(height), m_width(width)
{
	Load(fileName);
}

Texture::~Texture()
{
	if (m_internalBuffer != NULL)
	{
		delete m_internalBuffer;
		m_internalBuffer = NULL;
	}
}

/// <summary>Loads the texture from the path given as param.</summary>
/// <remark> Expects the file to be of bmp format, also expects to the file to be without padding so a multiply of 2.</remark> //TODO fix so it accepts padding
void Texture::Load(std::string fileName)
{

	unsigned char* dataBuffers[2] = { nullptr, nullptr }; // Header buffers

	BITMAPFILEHEADER* bmpHeader = nullptr; // Header
	BITMAPINFOHEADER* bmpInfo = nullptr; // Info 

	std::ifstream file(fileName, std::ios::binary);

	dataBuffers[0] = new unsigned char[sizeof(BITMAPFILEHEADER)];
	dataBuffers[1] = new unsigned char[sizeof(BITMAPINFOHEADER)];

	file.read((char*)dataBuffers[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)dataBuffers[1], sizeof(BITMAPINFOHEADER));

	bmpHeader = (BITMAPFILEHEADER*)dataBuffers[0];
	bmpInfo = (BITMAPINFOHEADER*)dataBuffers[1];

	m_internalBuffer = new unsigned char[bmpInfo->biSizeImage];

	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)m_internalBuffer, bmpInfo->biSizeImage);

	unsigned char tmpRGB = 0; 

	for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
	{
		tmpRGB = m_internalBuffer[i];
		m_internalBuffer[i] = m_internalBuffer[i + 2];
		m_internalBuffer[i + 2] = tmpRGB;
	}

	m_width = bmpInfo->biWidth;
	m_height = bmpInfo->biHeight;

	file.close();
}

/// <summary>Maps the texture position given as param to rgb values</summary>
ZyphMaths::Vector4 Texture::Map(float tu, float tv)
{
	if (m_internalBuffer == NULL)
	{
		return ZyphMaths::Vector4::ONE;
	}

	int u = abs((int)(tu * m_width) % m_width);
	int v = abs((int)(tv * m_height) % m_height);

	int pos = (u + v * m_width) * 3;

	char r = m_internalBuffer[pos + 0];
	char g = m_internalBuffer[pos + 1];
	char b = m_internalBuffer[pos + 2];

	return ZyphMaths::Vector4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}
