#define _CRT_SECURE_NO_WARNINGS

#include "device.h"
#include "ZyphMath.h"
#include <math.h>

/// <summary> Constructor</summary>
/// <remark> Will create the following:
/// A window with the windowname given as param.
/// A bitmap using the width and height of the window.
/// A backbuffer and a depthbuffer using the window/bitmap width and height.
/// Sets the fillmode to renderpoint.
/// </remark>
Device::Device(Window* window) :
	m_window(window)
{
	m_renderWidth = m_window->GetScreenWidth();
	m_renderHeight = m_window->GetScreenHeight();
	m_bitmap = new Bitmap(m_renderWidth, m_renderHeight);
	m_backBuffer = new char[m_renderWidth * m_renderHeight * 3];
	m_depthBuffer = new float[m_renderWidth * m_renderHeight];
	Render = &Device::RenderPoint;
}

Device::~Device()
{
	if (m_bitmap != NULL)
	{
		delete m_bitmap;
		m_bitmap = NULL;
	}
	if (m_backBuffer != NULL)
	{
		delete m_backBuffer;
		m_backBuffer = NULL;
	}
	if (m_depthBuffer != NULL)
	{
		delete m_depthBuffer;
		m_depthBuffer = NULL;
	}
}

/// <summary>Sets the fillmode that the program is going to use. defaults to point fillmode.</summary>
void Device::SetFillMode(ZyphSettings::ZyphSettings::FILLMODE drawType)
{
	switch (drawType)
	{
	case ZyphSettings::ZyphSettings::FILLMODE::LINE:
		Render = &Device::RenderLine;
		break;
	case ZyphSettings::ZyphSettings::FILLMODE::POINT:
		Render = &Device::RenderPoint;
		break;
	case ZyphSettings::ZyphSettings::FILLMODE::SOLID:
		Render = &Device::RenderSolid;
		break;
	default:
		Render = &Device::RenderPoint;
	}
}

/// <summary>Clears the backbuffer to the values given as param, also clears the depthbuffer to float max value.</summary>
/// <seealso cref="Zypmath::MAX_FLOAT_VALUE" />
void Device::Clear(char red, char green, char blue, char alpha)
{
	for (int i = 0; i < m_renderWidth *  m_renderHeight * 3; i += 3)
	{
		m_backBuffer[i] = blue;
		m_backBuffer[i + 1] = green;
		m_backBuffer[i + 2] = red;
	}

	// Clearing Depth Buffer
	for (int index = 0; index <  m_renderWidth * m_renderHeight; index++)
	{
		m_depthBuffer[index] = ZyphMaths::ZyphMath::MAX_FLOAT_VALUE;
	}
}

/// <summary> Set the backbuffers data to the frontbuffer and show it on the window handle.</summary>
void Device::Present()
{

	char* tempFileHeader = new char[sizeof(BITMAPFILEHEADER)];
	char* tempInfoHeader = new char[sizeof(BITMAPINFOHEADER)];

	BITMAPFILEHEADER* bmfh = (BITMAPFILEHEADER*)tempFileHeader;
	//memcpy(bmfh, m_bitmap->m_bitmapFileHeader, sizeof(BITMAPFILEHEADER));

	bmfh->bfOffBits = m_bitmap->m_bitmapFileHeader.bfOffBits;
	bmfh->bfReserved1 = m_bitmap->m_bitmapFileHeader.bfReserved1;
	bmfh->bfReserved2 = m_bitmap->m_bitmapFileHeader.bfReserved2;
	bmfh->bfSize = m_bitmap->m_bitmapFileHeader.bfSize;
	bmfh->bfType = m_bitmap->m_bitmapFileHeader.bfType;

	BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)tempInfoHeader;
	//memcpy(tempInfoHeader, m_bitmap->m_bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	bmih->biBitCount = m_bitmap->m_bitmapInfoHeader.biBitCount;
	bmih->biClrImportant = m_bitmap->m_bitmapInfoHeader.biClrImportant;
	bmih->biClrUsed = m_bitmap->m_bitmapInfoHeader.biClrUsed;
	bmih->biCompression = m_bitmap->m_bitmapInfoHeader.biCompression;
	bmih->biHeight = m_bitmap->m_bitmapInfoHeader.biHeight;
	bmih->biPlanes = m_bitmap->m_bitmapInfoHeader.biPlanes;
	bmih->biSize = m_bitmap->m_bitmapInfoHeader.biSize;
	bmih->biSizeImage = m_bitmap->m_bitmapInfoHeader.biSizeImage;
	bmih->biWidth = m_bitmap->m_bitmapInfoHeader.biWidth;
	bmih->biXPelsPerMeter = m_bitmap->m_bitmapInfoHeader.biXPelsPerMeter;
	bmih->biYPelsPerMeter = m_bitmap->m_bitmapInfoHeader.biYPelsPerMeter;

	BITMAPINFO* bmi = (BITMAPINFO*)bmih;

	char* bits;
	bits = (char*)(m_backBuffer + bmfh->bfOffBits);
	//Create a bitmap, using the bitmap settings.
	HBITMAP hBmp = CreateDIBitmap(GetDC(m_window->GetWindowHandleInstance()), bmih, CBM_INIT, bits, bmi, DIB_RGB_COLORS);
	HDC dcmem = CreateCompatibleDC(NULL);

	SelectObject(dcmem, hBmp);

	// blit it to the visible dc
	BitBlt(GetDC(m_window->GetWindowHandleInstance()), 0, 0, m_renderWidth, m_renderHeight, dcmem,
		0, 0, SRCCOPY);

	DeleteDC(dcmem);  // clear up the memory dc
}

/// <summary> </summary>
void Device::ProcessScanLine(ScanLineData data, Model::Vertex vertex1, Model::Vertex vertex2, Model::Vertex vertex3, Model::Vertex vertex4, ZyphMaths::Vector4 color, Texture* texture)
{
	float gradient1 = vertex1.coordinates.y != vertex2.coordinates.y ? (data.currentY - vertex1.coordinates.y) / (vertex2.coordinates.y - vertex1.coordinates.y) : 1;
	float gradient2 = vertex3.coordinates.y != vertex4.coordinates.y ? (data.currentY - vertex3.coordinates.y) / (vertex4.coordinates.y - vertex3.coordinates.y) : 1;

	float sx = ZyphMaths::ZyphMath::INTERPOLATE(vertex1.coordinates.x, vertex2.coordinates.x, gradient1);
	float ex = ZyphMaths::ZyphMath::INTERPOLATE(vertex3.coordinates.x, vertex4.coordinates.x, gradient2);

	//// starting Z & ending Z
	//float z1 = ZyphMaths::ZyphMath::INTERPOLATE(vertex1.coordinates.z, vertex2.coordinates.z, gradient1);
	//float z2 = ZyphMaths::ZyphMath::INTERPOLATE(vertex3.coordinates.z, vertex4.coordinates.z, gradient2);

	//float snl = ZyphMaths::ZyphMath::INTERPOLATE(data.ndotla, data.ndotlb, gradient1);
	//float enl = ZyphMaths::ZyphMath::INTERPOLATE(data.ndotlc, data.ndotld, gradient2);

	//// Interpolating texture coordinates on Y
	//float su = ZyphMaths::ZyphMath::INTERPOLATE(data.ua, data.ub, gradient1);
	//float eu = ZyphMaths::ZyphMath::INTERPOLATE(data.uc, data.ud, gradient2);
	//float sv = ZyphMaths::ZyphMath::INTERPOLATE(data.va, data.vb, gradient1);
	//float ev = ZyphMaths::ZyphMath::INTERPOLATE(data.vc, data.vd, gradient2);

	// drawing a line from left (sx) to right (ex)
	for (int x = sx; x < ex; x++)
	{
		DrawPoint(ZyphMaths::Vector3(x, data.currentY, 1.0f), color);

	/*	float gradient = (x - sx) / (ex - sx);

		float z = ZyphMaths::ZyphMath::INTERPOLATE(z1, z2, gradient);
		float ndotL = ZyphMaths::ZyphMath::INTERPOLATE(snl, enl, gradient);
		float u = ZyphMaths::ZyphMath::INTERPOLATE(su, eu, gradient);
		float v = ZyphMaths::ZyphMath::INTERPOLATE(sv, ev, gradient);

		ZyphMaths::Vector4 textureColor;

		if (texture != NULL)
		{
			textureColor = texture->Map(u, v);
		}
		else
		{
			textureColor = ZyphMaths::Vector4::ONE;
		}
*/
	}
}

/// <summary> Projects the 3d data given as params, to a 2d visualization. </summary>
/// <returns> Returns a vertex that holds all the 2d data, like coordinates, normal and texture values. </returns>
Model::Vertex Device::Project(Model::Vertex vertex, ZyphMaths::Matrix4x4 transMatrix, ZyphMaths::Matrix4x4 worldMatrix)
{
	ZyphMaths::Vector3 point2D = ZyphMaths::Vector3::TransformCoordinate(vertex.coordinates, transMatrix);

	float x = point2D.x * m_renderWidth + m_renderWidth / 2.0f;
	float y = -point2D.y * m_renderHeight + m_renderHeight / 2.0f;

	Model::Vertex temp = Model::Vertex();

	temp.coordinates = ZyphMaths::Vector3(x, y, point2D.z);
	temp.normal = ZyphMaths::Vector3::TransformCoordinate(vertex.normal, worldMatrix);
	temp.worldCoordinates = ZyphMaths::Vector3::TransformCoordinate(vertex.coordinates, worldMatrix);
	temp.texture = vertex.texture;

	return temp;
}

/// <summary> Projects the 3d data given as params, to a 2d visualization.</summary>
/// <returns> Returns the 2d coordinates. </returns>
ZyphMaths::Vector2 Device::Project(ZyphMaths::Vector3 coord, ZyphMaths::Matrix4x4 transMat)
{
	ZyphMaths::Vector3 point = ZyphMaths::Vector3::TransformCoordinate(coord, transMat);

	float x = point.x * m_renderWidth + m_renderWidth / 2.0f;
	float y = -point.y * m_renderHeight + m_renderHeight / 2.0f;

	return (ZyphMaths::Vector2(x, y));
}

/// <summary> Draws a point on the screen, if the point is within the screens borders, with the given color as param. </summary>
void Device::DrawPoint(ZyphMaths::Vector3& point, ZyphMaths::Vector4 color)
{
	if (point.x >= 0 && point.y >= 0 && point.x < m_renderWidth && point.y < m_renderHeight)
	{
		SetPixel((int)point.x, (int)point.y, point.z, color);
	}
}

/// <summary> Draws a line on the screen using bressingham algorithm. </summary>
void Device::DrawBline(ZyphMaths::Vector2 point0, ZyphMaths::Vector2 point1)
{
	int x0 = (int)point0.x;
	int y0 = (int)point0.y;
	int x1 = (int)point1.x;
	int y1 = (int)point1.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while ((x0 != x1) && (y0 != y1)) {
		DrawPoint(ZyphMaths::Vector3(x0, y0, 1.0f), ZyphMaths::Vector4::ONE);

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

/// <summary> Draws a triangle on the screen. </summary>
void Device::DrawTriangle(Model::Vertex vertex1, Model::Vertex vertex2, Model::Vertex vertex3, ZyphMaths::Vector4 color, Texture* texture)
{
	float dP1P2 = 0.0f;
	float dP1P3 = 0.0f;

	if (vertex1.coordinates.y > vertex2.coordinates.y)
	{
		Model::Vertex temp = vertex2;
		vertex2 = vertex1;
		vertex1 = temp;
	}

	if (vertex2.coordinates.y > vertex3.coordinates.y)
	{
		Model::Vertex temp = vertex2;
		vertex2 = vertex3;
		vertex3 = temp;
	}

	if (vertex1.coordinates.y > vertex2.coordinates.y)
	{
		Model::Vertex temp = vertex2;
		vertex2 = vertex1;
		vertex1 = temp;
	}

	ZyphMaths::Vector3 lightPos = ZyphMaths::Vector3(0.0f, 10.0f, 10.0f);
	// computing the cos of the angle between the light vector and the normal vector
	// it will return a value between 0 and 1 that will be used as the intensity of the color
	float nl1 = ZyphMaths::ZyphMath::COMPUTENDOTL(vertex1.worldCoordinates, vertex1.normal, lightPos);
	float nl2 = ZyphMaths::ZyphMath::COMPUTENDOTL(vertex2.worldCoordinates, vertex2.normal, lightPos);
	float nl3 = ZyphMaths::ZyphMath::COMPUTENDOTL(vertex3.worldCoordinates, vertex3.normal, lightPos);

	ScanLineData data = ScanLineData();
	
	if (vertex2.coordinates.y > vertex1.coordinates.y)
	{
		dP1P2 = (vertex2.coordinates.x - vertex1.coordinates.x) / (vertex2.coordinates.y - vertex1.coordinates.y);
	}

	if (vertex3.coordinates.y > vertex1.coordinates.y)
	{
		dP1P3 = (vertex3.coordinates.x - vertex1.coordinates.x) / (vertex3.coordinates.y - vertex1.coordinates.y);
	}

	if (dP1P2 > dP1P3)
	{
		for (int y = (int)vertex1.coordinates.y; y <= (int)vertex3.coordinates.y; y++)
		{
			data.currentY = y;
			if (y < vertex2.coordinates.y)
			{
				data.ndotla = nl1;
				data.ndotlb = nl3;
				data.ndotlc = nl1;
				data.ndotld = nl2;

				data.ua = vertex1.texture.x * -1.0f;
				data.ub = vertex3.texture.x * -1.0f;
				data.uc = vertex1.texture.x * -1.0f;
				data.ud = vertex2.texture.x * -1.0f;

				data.va = vertex1.texture.y;
				data.vb = vertex3.texture.y;
				data.vc = vertex1.texture.y;
				data.vd = vertex2.texture.y;

				ProcessScanLine(data, vertex1, vertex3, vertex1, vertex2, color, texture);
			}
			else
			{
				data.ndotla = nl1;
				data.ndotlb = nl3;
				data.ndotlc = nl2;
				data.ndotld = nl3;

				data.ua = vertex1.texture.x * -1.0f;
				data.ub = vertex3.texture.x * -1.0f;
				data.uc = vertex2.texture.x * -1.0f;
				data.ud = vertex3.texture.x * -1.0f;

				data.va = vertex1.texture.y;
				data.vb = vertex3.texture.y;
				data.vc = vertex2.texture.y;
				data.vd = vertex3.texture.y;

				ProcessScanLine(data, vertex1, vertex3, vertex2, vertex3, color, texture);
			}
		}
	}
	else
	{
		for (int y = (int)vertex1.coordinates.y; y <= (int)vertex3.coordinates.y; y++)
		{
			data.currentY = y;
			if (y < vertex2.coordinates.y)
			{
				data.ndotla = nl1;
				data.ndotlb = nl2;
				data.ndotlc = nl1;
				data.ndotld = nl3;

				data.ua = vertex1.texture.x;
				data.ub = vertex2.texture.x;
				data.uc = vertex1.texture.x;
				data.ud = vertex3.texture.x;

				data.va = vertex1.texture.y;
				data.vb = vertex2.texture.y;
				data.vc = vertex1.texture.y;
				data.vd = vertex3.texture.y;

				ProcessScanLine(data, vertex1, vertex2, vertex1, vertex3, color, texture);
			}
			else
			{
				data.ndotla = nl2;
				data.ndotlb = nl3;
				data.ndotlc = nl1;
				data.ndotld = nl3;

				data.ua = vertex2.texture.x;
				data.ub = vertex3.texture.x;
				data.uc = vertex1.texture.x;
				data.ud = vertex3.texture.x;

				data.va = vertex2.texture.y;
				data.vb = vertex3.texture.y;
				data.vc = vertex1.texture.y;
				data.vd = vertex3.texture.y;

				ProcessScanLine(data, vertex2, vertex3, vertex1, vertex3, color, texture);
			}
		}
	}
}

/// <summary> Sets the pixel color for the pixel with the x,y and z loc given as param (z is used to check if the pixel needs to be drawn or not).</summary>
/// <remark> The color of the pixel is the color given as param.</remark>
void Device::SetPixel(int x, int y, float z, ZyphMaths::Vector4 color)
{
	if (m_depthBuffer[x + y * m_renderWidth] < z)
	{
		return;
	}

	m_depthBuffer[x + y * m_renderWidth] = z;

	m_backBuffer[((x + y * m_renderWidth) * 3) + 2] = (char)(color.x * 255);
	m_backBuffer[((x + y * m_renderWidth) * 3) + 1] = (char)(color.y * 255);
	m_backBuffer[((x + y * m_renderWidth) * 3)] = (char)(color.z * 255);
}

/// <summary> Renders the model, by drawing the whole model and fill it with the color, or texture if available. </summary>
void Device::RenderSolid(Camera camera, Model models[], int modelCount)
{
	ZyphMaths::Matrix4x4 viewMatrix = ZyphMaths::Matrix4x4::LookAtLH(camera.m_position, camera.m_target, ZyphMaths::Vector3::UPVECTOR);
	ZyphMaths::Matrix4x4 projectionMatrix = ZyphMaths::Matrix4x4::PerspectiveFovLH(ZyphMaths::ZyphMath::PI / 4, (float)m_renderWidth / m_renderHeight, 0.01f, 1.0f);
	for (int i = 0; i < modelCount; i++)
	{
		ZyphMaths::Matrix4x4 worldMatrix = ZyphMaths::Matrix4x4::RotationYawPitchRoll(models[i].m_rotation.y, models[i].m_rotation.x, models[i].m_rotation.z) * ZyphMaths::Matrix4x4::Translation(models[i].m_position);
		ZyphMaths::Matrix4x4 transformMatrix = worldMatrix * viewMatrix * projectionMatrix;
		for (int j = 0; j < models[i].m_vertexCount; j += 3)
		{
			Model::Vertex vertex1 = Project(models[i].m_vertexStructure[j + 0], transformMatrix, worldMatrix);
			Model::Vertex vertex2 = Project(models[i].m_vertexStructure[j + 1], transformMatrix, worldMatrix);
			Model::Vertex vertex3 = Project(models[i].m_vertexStructure[j + 2], transformMatrix, worldMatrix);

			DrawTriangle(vertex1, vertex2, vertex3, ZyphMaths::Vector4::ONE, models[i].m_texture);
		}
	}
}

/// <summary> Renders the model, by drawing the lines between the points of the model. </summary>
void Device::RenderLine(Camera camera, Model models[], int modelCount)
{
	ZyphMaths::Matrix4x4 viewMatrix = ZyphMaths::Matrix4x4::LookAtLH(camera.m_position, camera.m_target, ZyphMaths::Vector3::UPVECTOR);
	ZyphMaths::Matrix4x4 projectionMatrix = ZyphMaths::Matrix4x4::PerspectiveFovLH(ZyphMaths::ZyphMath::PI / 4, (float)m_renderWidth / m_renderHeight, 0.01f, 1.0f);
	for (int i = 0; i < modelCount; i++)
	{
		ZyphMaths::Matrix4x4 worldMatrix = ZyphMaths::Matrix4x4::RotationYawPitchRoll(models[i].m_rotation.y, models[i].m_rotation.x, models[i].m_rotation.z) * ZyphMaths::Matrix4x4::Translation(models[i].m_position);
		ZyphMaths::Matrix4x4 transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

		for (int j = 0; j < models[i].m_vertexCount; j += 3)
		{
			ZyphMaths::Vector3 vec3 = ZyphMaths::Vector3::TransformNormal(models[i].m_vertexStructure[j + 0].normal, (worldMatrix * viewMatrix));

			if (vec3.z >= 0)
			{
				break;
			}

			ZyphMaths::Vector2 point1 = Project(models[i].m_vertexStructure[j + 0].coordinates, transformMatrix);
			ZyphMaths::Vector2 point2 = Project(models[i].m_vertexStructure[j + 1].coordinates, transformMatrix);
			ZyphMaths::Vector2 point3 = Project(models[i].m_vertexStructure[j + 2].coordinates, transformMatrix);

			DrawBline(point1, point2);
			DrawBline(point2, point3);
			DrawBline(point3, point1);
		}
	}
}

/// <summary>Renders the model, by drawing the points of the model. </summary>
void Device::RenderPoint(Camera camera, Model models[], int modelCount)
{
	ZyphMaths::Matrix4x4 viewMatrix = ZyphMaths::Matrix4x4::LookAtLH(camera.m_position, camera.m_target, ZyphMaths::Vector3::UPVECTOR);
	ZyphMaths::Matrix4x4 projectionMatrix = ZyphMaths::Matrix4x4::PerspectiveFovLH(ZyphMaths::ZyphMath::PI / 4, (float)m_renderWidth / m_renderHeight, 0.01f, 1.0f);
	for (int i = 0; i < modelCount; i++)
	{
		ZyphMaths::Matrix4x4 worldMatrix = ZyphMaths::Matrix4x4::RotationYawPitchRoll(models[i].m_rotation.y, models[i].m_rotation.x, models[i].m_rotation.z) * ZyphMaths::Matrix4x4::Translation(models[i].m_position);
		ZyphMaths::Matrix4x4 transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

		for (int j = 0; j < models[i].m_vertexCount; j += 3)
		{
			ZyphMaths::Vector2 point1 = Project(models[i].m_vertexStructure[j + 0].coordinates, transformMatrix);

			DrawPoint(ZyphMaths::Vector3(point1.x, point1.y, 1.0f), ZyphMaths::Vector4::ONE);
		}
	}
}


