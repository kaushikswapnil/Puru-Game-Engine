#pragma once
#include"TextureClass.h"

class puruTexture
{
public:
	puruTexture();
	puruTexture(WCHAR* fileName, int width, int height);
	puruTexture(const puruTexture& other) = delete;
	~puruTexture();

	puruTexture& operator=(const puruTexture& other) = delete;

	bool Initialize(WCHAR* fileName, int width, int height);

	void Shutdown();

	int GetWidth() { return m_TextureWidth; }
	int GetHeight() { return m_TextureHeight; }

	ID3D11ShaderResourceView* GetTexture();

private:
	TextureClass* m_Texture;
	int m_TextureWidth, m_TextureHeight;
};

