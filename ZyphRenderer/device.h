#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "window.h" 
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "camera.h"
#include "bitmap.h"
#include "model.h"
#include "scanlinedata.h"
#include "zyphsettings.h"

class Device
{
private:
	char* m_backBuffer; // Char array to hold the backbuffer data
	float* m_depthBuffer; // Float array to hold the depth data
	Bitmap* m_bitmap; // Bitmap that holds the current frontbuffer data.
	Window* m_window; // Window handle.
	int m_renderWidth; // Hold the window width (performance increase).
	int m_renderHeight; // Hold the window height (performance increase).

	void ProcessScanLine(ScanLineData data, Model::Vertex vertex1, Model::Vertex vertex2, Model::Vertex vertex3, Model::Vertex vertex4, ZyphMaths::Vector4 color, Texture* texture);
	Model::Vertex Project(Model::Vertex coord, ZyphMaths::Matrix4x4 transMatrix, ZyphMaths::Matrix4x4 worldMatrix); // Projects the 3d data given as params, to a 2d visualization.
	ZyphMaths::Vector2 Device::Project(ZyphMaths::Vector3 coord, ZyphMaths::Matrix4x4 transMat); // Projects the 3d data given as params, to a 2d visualization.
	void DrawPoint(ZyphMaths::Vector3& point, ZyphMaths::Vector4 color); // Draws a point on screen with the given color as param.
	void DrawBline(ZyphMaths::Vector2 point0, ZyphMaths::Vector2 point1); // Draws a line on the screen using bressingham algorithm.
	void DrawTriangle(Model::Vertex vertex1, Model::Vertex vertex2, Model::Vertex vertex3, ZyphMaths::Vector4 color, Texture* texture); // Draws a triangle on the screen.
	void SetPixel(int x, int y, float z, ZyphMaths::Vector4 color); // Sets the pixel color for the pixel with the x y and z loc given as param. (z is used to check if it needs to be drawn or not).
	void RenderSolid(Camera camera, Model model[], int modelCount); // Renders the model, by drawing the whole model and fill it with a color, or texture if available.
	void RenderLine(Camera camera, Model model[], int modelCount); // Renders the model, by  drawing the lines between the points of the model.
	void RenderPoint(Camera camera, Model model[], int modelCount); // Renders the model, by drawing the points of the model. 

public:
	Device(Window* window); // Default Constructor. 
	virtual ~Device();
    void(Device::*Render)(Camera camera, Model models[], int modelCount) = NULL; // Function pointer to one of the render functions.
	void SetFillMode(ZyphSettings::ZyphSettings::FILLMODE drawType); // Sets the fillmode that the program is going to use.

	void Clear(char red, char green, char blue, char alpha); // Clears the backbuffer to the values given as params, also clears the depthbuffer.
	void Present(); // Sets the backbuffer data into the frontbuffer and shows it on the window handle

};

#endif
