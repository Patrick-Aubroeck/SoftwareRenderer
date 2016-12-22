#include "window.h"

//forward declaration.
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

/// <summary> standard constructor, creates a window with the values set to zero/NULL</summary>
Window::Window(std::string windowName) :
	m_windowName(windowName),
	m_hinstance(NULL),
	m_hwnd(NULL),
	m_fullScreen(false),
	m_screenHeight(0),
	m_screenWidth(0)
{
	Initialize();
}

/// <summary>standard destructor call shutdown before delete</summary>
Window::~Window()
{

}

/// <summary> initialize a window with pre defined settings</summary>
/// <seealso cref="CreateWindowEx"/>
void Window::Initialize()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;

	/// set fullscreen on false, will have to do something about this later.
	m_fullScreen = false;
	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	std::string applicationName = "SoftwareRenderer";

	//// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_fullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = 0;
		posY = 0;
	}
	else
	{
		// If windowed then set it to 640x480 resolution.
		m_screenWidth = 800;
		m_screenHeight = 640;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, applicationName.c_str(), m_windowName.c_str(),
		WS_BORDER | CW_DEFAULT,
		posX, posY, m_screenWidth, m_screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

}

/// <summary>shutdown the window that calls this function</summary>
/// <remark>before deleting will set the fullscreensetting to false, and will remove the application instance</remark>
/// <remark> Change the display settings to zero if fullscreen was activeat the time of calling the shutdown function </remark>
/// <seealso cref="UnregisterClass()"/>
/// <seealso cref="DestroyWindow()"/>
void Window::Shutdown()
{
	// Fix the display settings if leaving full screen mode.
	if (m_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass("SofwareRenderer", m_hinstance);
	m_hinstance = NULL;
}

/// <summary>get the screenheight of the window that calls this function</summary>
/// <returns> return the screenheight as an integer </returns>
const int Window::GetScreenHeight()
{
	return m_screenHeight;
}

/// <summary>get the screenwidth of the window that calls this function</summary>
/// <returns> return the screenwidth as an integer </returns>
const int Window::GetScreenWidth()
{
	return m_screenWidth;
}

/// <summary>get the windowname of the window that calls this function</summary>
/// <returns>return the windowname as a (std)string </returns>
const std::string Window::GetWindowName()
{
	return m_windowName;
}

/// <summary>get the handle to the window that calls this function</summary>
/// <returns>returns the handle to this window as HWND </returns>
const HWND Window::GetWindowHandleInstance()
{
	return m_hwnd;
}

/// <summary> get the fullscreensetting of the window that calls this function</summary>
/// <returns> returns the fullscreensetting of the window as a boolean ( if fullscreen return yes )</returns>
const bool Window::GetFullScreenSetting()
{
	return m_fullScreen;
}

/// <summary>get if the window that calls this function is the active window</summary>
/// <returns> returns true if the window is the active window(foreground window), otherwise return false </returns>
/// <seealso cref="GetForegroundWindow()"/>
const bool Window::GetIsActive()
{
	HWND activeHwnd = GetForegroundWindow();
	if (m_hwnd == activeHwnd)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/// <summary>updates the window pump message</summary>
/// <remark> call this function every cycle so it will read the window messages</remark>
/// <seealso cref="PeekMessage()" />
/// <seealso cref="TranslateMessage()" />
/// <seealso cref="DispatchMessage()" />
void Window::UpdateWindow()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umessage, wparam, lparam);
}


