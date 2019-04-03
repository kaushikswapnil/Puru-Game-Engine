#pragma once

//Sprite class that is initialized using textures

#include"puruQuad.h"

class puruTexture;

class puruSprite
{
public:
	puruSprite();
	puruSprite(float texLeft, float texTop, int width, int height, puruTexture* texture);
	puruSprite(const puruSprite& copyValue);
	~puruSprite();

	puruSprite& operator=(const puruSprite& other);

	bool Initialize(float texLeft, float texTop, int width, int height, puruTexture* texture);

	void Render(int x, int y, float scale = 1.0f);

	void Shutdown();

	puruTexture* GetTexture();

private:
	puruQuad* m_quad;
};

