#pragma once
#include"CCriticalSection.h"

//#####################
//Singleton - Documentation
//#####################
//Declare the class and add pge_DECLARE_SINGLETON_SIMPLE macro inside. Make your own destructor.

#define pge_DECLARE_SINGLETON_SIMPLE_CC(_Classname, _CallConv) \
public: \
	_Classname(const _Classname&) = delete; \
	_Classname& operator=(const _Classname&) = delete; \
	\
	_CallConv static _Classname& GetInstance() \
	{ \
		static _Classname instance; \
		return instance; \
		\
	} \
    _CallConv static _Classname* GetInstancePtr() \
	{ \
		return &GetInstance(); \
		\
	} \
	\
private: \
	_Classname() = default; \

#define pge_DECLARE_SINGLETON_SIMPLE(_Classname) pge_DECLARE_SINGLETON_SIMPLE_CC(_Classname, )


//////////////////////Keeping the old class for dependencies #TODO

class Singleton
{
public:
	Singleton();

	virtual ~Singleton();

protected:
	static CCriticalSection m_cs;
};
