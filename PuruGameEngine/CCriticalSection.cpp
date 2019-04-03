#include "CCriticalSection.h"

CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection(&m_cs);
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(&m_cs);
}

void CCriticalSection::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_cs);
}
