#include "fpscounter.h"

FpsCounter::FpsCounter()	
{
	m_fps = 0;
	m_count = 0;
	m_totalfps = 0;
	m_startTime = timeGetTime();
	m_beginTime = timeGetTime();
}

FpsCounter::~FpsCounter()
{
}

void FpsCounter::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_totalfps += m_fps;
		m_count = 0;
		m_startTime = timeGetTime();
	}

}

int FpsCounter::GetFps()
{
	return m_fps;
}

float FpsCounter::GetAvgFps()
{
	if (m_count == 0 || m_totalfps == 0)
		return m_fps;
	return m_totalfps / m_count;
}
