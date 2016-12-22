#ifndef _FPSCOUNTER_H_
#define _FPSCOUNTER_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class FpsCounter
{
public:
	FpsCounter();
	~FpsCounter();
	void Frame();
	int GetFps();
	float GetAvgFps();
private:
	int m_fps;
	int m_totalfps;
	float m_avgfps;
	int m_count;
	unsigned long m_startTime;
	unsigned long m_beginTime;
};

#endif // !_FPSCOUNTER_H_
