#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "device.h"

class Kernel 
{
private:
	Device* m_device; // The device.
	Window* m_window;

public:
	Kernel(); // Constructor, Also creates a device
	virtual ~Kernel(); // Destructor
	void Run(); // Starts the while loop that is needed to create the window/image/scene etc.
	Device* GetDevice(); // Returns the device.
	Window* GetWindow();
};
#endif 

