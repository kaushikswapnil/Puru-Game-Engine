#include "SoundClass.h"



SoundClass::SoundClass() : m_DirectSound(nullptr), m_primaryBuffer(nullptr)
{
}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize(HWND hwnd)
{
	bool result;

	//Initialize the direct sound and primary buffer
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	ShutdownDirectSound();
}

IDirectSound8 * SoundClass::GetDirectSoundInterface()
{
	return m_DirectSound;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	//Initialize DirectSound interface pointer 
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set the cooperative level to priority so the format of the
	//primary sound buffer can be modified
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	//Set up desc of access to primary buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Get control of the primary sound buffer on default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Setup the format of primary sound buffer
	//In this case it is a .wav
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//set primary buffer to the wave format specified
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = nullptr;
	}

	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = nullptr;
	}
}
