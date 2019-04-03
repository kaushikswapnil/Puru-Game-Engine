#pragma once

#include<windows.h>

class CCriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();

	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_cs;
};