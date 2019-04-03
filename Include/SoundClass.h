#pragma once

//Linking
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
public:
	SoundClass();
	SoundClass(const SoundClass&) = delete;
	~SoundClass();

	SoundClass& operator=(const SoundClass&) = delete;

	bool Initialize(HWND);
	void Shutdown();

	IDirectSound8* GetDirectSoundInterface();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
};

