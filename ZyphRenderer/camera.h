#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3.h"

class Camera
{
public:
	ZyphMaths::Vector3 m_position; // The position of the camera.
	ZyphMaths::Vector3 m_target; // The target the camera is looking at.

	Camera(); // Constructor.
	virtual ~Camera(); // Desctructor.
	
};

#endif 
