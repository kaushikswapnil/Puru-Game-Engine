#pragma once
#include"CCriticalSection.h"

//#####################
//Singleton - Documentation
//#####################
//Declare the class and add this macro inside. Make your own destructor.

#define pge_SINGLETON_SIMPLE(_Classname) \
public: \
	_Classname(const _Classname&) = delete; \
	_Classname& operator=(const _Classname&) = delete; \
	\
	static _Classname* GetInstance() \
	{ \
		static _Classname m_Instance; \
		return &m_Instance; \
		\
	} \
	\
private: \
	_Classname() = default;


