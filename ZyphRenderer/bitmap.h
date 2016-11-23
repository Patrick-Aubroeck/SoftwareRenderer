#ifndef _BITMAP_H_
#define _BITMAP_H_

class Bitmap
{
public:
	struct ZYPHBITMAPFILEHEADER              /**** BMP file header structure ****/
	{
		unsigned const short bfType = 0x4d42; // Used to set the type of the bitmap = 0x4d42(BM). Do not change!!
		unsigned int   bfSize;           /* Size of file */
		unsigned short bfReserved1;      /* Reserved */
		unsigned short bfReserved2;      /* ... */
		unsigned int   bfOffBits;        /* Offset to bitmap data */
	};

	struct ZYPHBITMAPINFOHEADER              /**** BMP file info structure ****/
	{
		unsigned int   biSize;           /* Size of info header */
		int            biWidth;          /* Width of image */
		int            biHeight;         /* Height of image */
		unsigned short biPlanes;         /* Number of color planes */
		unsigned short biBitCount;       /* Number of bits per pixel */
		unsigned int   biCompression;    /* Type of compression to use */
		unsigned int   biSizeImage;      /* Size of image data */
		int            biXPelsPerMeter;  /* X pixels per meter */
		int            biYPelsPerMeter;  /* Y pixels per meter */
		unsigned int   biClrUsed;        /* Number of colors used */
		unsigned int   biClrImportant;   /* Number of important colors */
	};

	ZYPHBITMAPFILEHEADER m_bitmapFileHeader; // Link to the fileheaderstruct
	ZYPHBITMAPINFOHEADER m_bitmapInfoHeader; // Link to the fileinfostruct
	Bitmap(int width, int height); // Default constructor, sets the width and height of the bitmap according to the width and height given as params.
	virtual ~Bitmap(); // Destructor

};

#endif 