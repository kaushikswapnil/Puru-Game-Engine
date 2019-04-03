#pragma once

//High precision timer that measures the exact time between frames of execution

#include<windows.h>

class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();
	float GetTimeinMs();

	void OutsideFrame();
	float GetLastCallTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;

	INT64 m_lastCallTime;
	float m_lastCallFrameTime;
};

