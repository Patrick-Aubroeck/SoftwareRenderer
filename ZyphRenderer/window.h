#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>
#include <string>

class Window
{
private:
	int m_screenWidth; // the screenwidth of the window
	int m_screenHeight; // the screenheight of the window
	bool m_fullScreen; // the fullscreen setting of the window
	HWND m_hwnd; // a handle to the window
	std::string m_windowName; // the window name
	HINSTANCE m_hinstance; // a instance to the window

public:
	Window(std::string windowName); //constructor, create a window with the given name 
	virtual ~Window(); // destructor
	void UpdateWindow(); // checks if there has been a message send, if there is a message translate it.
	void Initialize(); // creates the window with all its attributes
	void Shutdown(); // shut down its members

	const int GetScreenWidth(); // returns the width
	const int GetScreenHeight(); //returns the height
	const bool GetFullScreenSetting(); // returns the fullscreen setting
	const bool GetIsActive();  // returns if this window is active (foreground)
	const HWND GetWindowHandleInstance(); // returns the hwnd
	const std::string GetWindowName(); // returns the windowname


};
#endif 

