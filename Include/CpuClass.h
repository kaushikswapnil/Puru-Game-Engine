#pragma once

//The CpuClass is used to determine the percentage of total cpy use that is occurring each second

//Linking
#pragma comment(lib, "pdh.lib")

//Inludes
#include<pdh.h>

class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

