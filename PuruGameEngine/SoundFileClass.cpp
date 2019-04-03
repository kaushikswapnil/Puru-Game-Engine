#include "SoundFileClass.h"



SoundFileClass::SoundFileClass() : m_secondaryBuffer(nullptr)
{
}

SoundFileClass::SoundFileClass(char * filename, IDirectSound8* directSound) : m_secondaryBuffer(nullptr)
{
}

SoundFileClass::~SoundFileClass()
{
	Shutdown();
}

bool SoundFileClass::Initialize(char * filename, IDirectSound8* directSound)
{
	LoadWaveFile(filename, &m_secondaryBuffer, directSound);
	return true;
}

void SoundFileClass::Shutdown()
{
	ShutdownWaveFile(&m_secondaryBuffer);
}

bool SoundFileClass::LoadWaveFile(char * filename, IDirectSoundBuffer8 ** secondaryBuffer, IDirectSound8* directSound)
{
	int error;
	FILE *filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	//Open the wave file in binary
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	//Read in the wave file header
	count = fread(&waveFileHeader, sizeof(WaveHeaderType), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//Check that the chunk ID is the RIFF format
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I')
		|| (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	//Check that the sub chunk ID is the fmt format
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	//Check the audio format is WAVE_FORMAT_PCM
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	//Check that the wave file was recorded in stereo format
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	//Check that file ws recorded at a sample rate of 44.1 KHz
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	//Ensure that the wave file was recorded in 16 bit format
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	//Check the data chunk header
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	//Now that the wave header has been verified we can setup the secondary buffer we will load
	//the audio file onto. We first have to set the wave format and buffer desc of second buffer
	//similarly to primary buffer. However dwFlags and dwBufferBytes is different for secondary

	//Set the wave format of the secondary buffer
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set the buffer desc of secondary sound buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//The way to create create secondary buffer is to create a temporary idirectsoundbuffer with the
	//sound buffer desc. If this succeeds we use the temp buffer to create idirectsoundbuffer8 by
	//calling the query interface with the IID_IDIRECTSOUNDBUFFER8 parameter

	//Create the temporary sound buffer with the specific buffer settings
	result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Test the buffer format against the direct sound 8 interface and create the secondary buffer
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release the temp buffer
	tempBuffer->Release();
	tempBuffer = nullptr;

	//Now that the secondary buffer is ready, we can load in the wave data from the audio file. 
	//First I load it into a memory buffer so I can check and modify the data if I need to. Once 
	//the data is in memory, lock the buffer, copy the data, and then unlock it

	//Move to beginning of the wave data which starts at end of data chunk header
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	//Create a temp buffer to hold the wave data
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	//Read in the wave file data into the newly created buffer
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	//Close the file once done reading
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//Lock the secondary buffer to write wave data into it
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	//Copy wav data into buffer
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	//Unlock the secondary buffer after data has been written to it
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	//Release the wave data since it was copied into the secondary buffer
	delete[] waveData;
	waveData = nullptr;

	return true;
}

void SoundFileClass::ShutdownWaveFile(IDirectSoundBuffer8 ** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

bool SoundFileClass::PlayWaveFile()
{
	HRESULT result;

	//Set the position at the beginning of sound buffer
	result = m_secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	//Set the volume of the buffer to 100%
	result = m_secondaryBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	//Play the contents of secondary sound buffer
	result = m_secondaryBuffer->Play(0, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
