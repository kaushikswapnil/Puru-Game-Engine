#pragma once
#include"CriticalSection.h"

template <typename TClassName>
class Singleton
{
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

	

public:
	static TClassName& GetInstancePure() {
		static TClassName instance;
		return instance;
	}

	static TClassName* GetInstance() {
		return &(GetInstancePure());
	}
};

