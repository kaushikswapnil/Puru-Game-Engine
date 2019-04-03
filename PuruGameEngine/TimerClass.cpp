#include "TimerClass.h"



TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass &)
{
}


TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	//Check to see if this system supports high performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	//Find out how many times the frequency counter ticks every millisecond
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_lastCallTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;
	m_lastCallTime = currentTime;
}

float TimerClass::GetTimeinMs()
{
	return m_frameTime;
}

float TimerClass::GetTime()
{
	return (m_frameTime/1000);
}

void TimerClass::OutsideFrame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_lastCallTime);

	m_lastCallFrameTime = timeDifference / (1000*m_ticksPerMs);

	m_lastCallTime = currentTime;
}

float TimerClass::GetLastCallTime()
{
	return m_lastCallFrameTime;
}
