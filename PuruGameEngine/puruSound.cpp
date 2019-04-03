#include "puruSound.h"
#include "PuruGameSystem.h"
#include "SoundClass.h"
#include "SoundFileClass.h"

puruSound::puruSound() : m_sound(nullptr)
{
}

puruSound::puruSound(char * filename) : m_sound(nullptr)
{
	Initialize(filename);
}


puruSound::~puruSound()
{
	Shutdown();
}

bool puruSound::Initialize(char * filename)
{
	bool result;
	PGE* pge = PGE::GetInstance();
	SoundClass* snd = pge->GetSound();

	m_sound = new SoundFileClass();
	if (!m_sound)
	{
		m_sound = nullptr;
		return false;
	}

	result = m_sound->Initialize(filename, snd->GetDirectSoundInterface());
	if (!result)
	{
		m_sound->Shutdown();
		delete m_sound;
		m_sound = nullptr;
		return false;
	}
	
	return true;
}

bool puruSound::Initialize(const char* filename)
{
	char* cFilename = const_cast<char*>(filename);
	return Initialize(cFilename);
}

void puruSound::Shutdown()
{
	if (m_sound)
	{
		m_sound->Shutdown();
		delete m_sound;
		m_sound = nullptr;
	}
}

void puruSound::Play()
{
	if(m_sound)
	{
		m_sound->PlayWaveFile();
	}
}

void puruSound::PlayEx()
{
}
