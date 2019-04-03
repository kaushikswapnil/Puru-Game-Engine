#pragma once

class SoundFileClass;

class puruSound
{
public:
	puruSound();
	puruSound(char* filename);
	puruSound(const puruSound& other) = delete;
	~puruSound();

	puruSound& operator=(const puruSound& other) = delete;

	bool Initialize(char* filename);
	bool Initialize(const char* filename);
	void Shutdown();

	void Play();
	void PlayEx();

private:
	SoundFileClass* m_sound;
};

