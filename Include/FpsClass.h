#pragma once

//The FPS class is a counter with associated timer. It counts how many 
//frames occur in a one second period and constantly updates that count

//Linking
#pragma comment(lib, "winmm.lib")

//Includes
#include <windows.h>
#include <mmsystem.h>

class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass&);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

