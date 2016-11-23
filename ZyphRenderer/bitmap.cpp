#include "bitmap.h"

/// <summary> Default constructor, sets the width and height of the bitmap according to the width and height given as params.</summary>
Bitmap::Bitmap(int width, int height)
{
	m_bitmapFileHeader.bfReserved1 = 0;
	m_bitmapFileHeader.bfReserved2 = 0;
	m_bitmapFileHeader.bfSize = 2 + sizeof(ZYPHBITMAPFILEHEADER) + sizeof(ZYPHBITMAPINFOHEADER) + width * height * 3;
	m_bitmapFileHeader.bfOffBits = 0x36;

	m_bitmapInfoHeader.biSize = sizeof(ZYPHBITMAPINFOHEADER);
	m_bitmapInfoHeader.biWidth = width;
	m_bitmapInfoHeader.biHeight = height;
	m_bitmapInfoHeader.biPlanes = 1;
	m_bitmapInfoHeader.biBitCount = 24;
	m_bitmapInfoHeader.biCompression = 0;
	m_bitmapInfoHeader.biSizeImage = 0;
	m_bitmapInfoHeader.biXPelsPerMeter = 5000;
	m_bitmapInfoHeader.biYPelsPerMeter = 5000;
	m_bitmapInfoHeader.biClrUsed = 0;
	m_bitmapInfoHeader.biClrImportant = 0;
}

Bitmap::~Bitmap()
{
}
