#pragma once
#include"CCriticalSection.h"

class Singleton
{
public:
	Singleton();
	virtual ~Singleton();

protected:
	static CCriticalSection m_cs;
};
