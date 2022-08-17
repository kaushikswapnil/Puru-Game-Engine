#pragma once

#include<windows.h>

class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_cs;
};

class AutoCriticalSection : public CriticalSection
{
public:
	AutoCriticalSection();
	~AutoCriticalSection();
};