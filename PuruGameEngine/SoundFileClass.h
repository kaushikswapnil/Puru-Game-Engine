#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundFileClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
public:
	SoundFileClass();
	SoundFileClass(char* filename, IDirectSound8* directSound);
	SoundFileClass(const SoundFileClass& other) = delete;
	~SoundFileClass();

	SoundFileClass& operator=(const SoundFileClass&) = delete;

	bool Initialize(char* filename, IDirectSound8* directSound);
	void Shutdown();

	bool LoadWaveFile(char*filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound8* directSound);
	void ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer);

	bool PlayWaveFile();

private:
	IDirectSoundBuffer8* m_secondaryBuffer;
};

