#pragma once
#include"puruSprite.h"

class puruAnim
{
public:
	puruAnim();
	puruAnim(int Frames, int nFPS, float texLeft, float texTop, int width, int height, puruTexture* texture);
	puruAnim(const puruAnim& other);
	~puruAnim();

	puruAnim& operator=(const puruAnim& other);

	bool Initialize(int Frames, int nFPS, float texLeft, float texTop, int width, int height, puruTexture* texture);
	void Shutdown();

	void Update(float elapsedTime);
	void Render(int x, int y, float scale = 1.0f);

	void Play();
	void Stop();

	void SetLoopMode(bool value);

	bool IsPlaying();

private:
	void ChangeSprite();

private:
	puruSprite* m_Sprite;
	int m_nFrames;
	int m_nFPS;
	float m_dtChangeTime;
	float m_dtCurrentTime;
	bool m_bPlaying;
	bool m_bLoop;
	float m_texLeft, m_texTop, m_texStartLeft, m_texStartTop;
	int m_width, m_height;

	int m_curFrame;
};

