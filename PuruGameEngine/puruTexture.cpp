#include "puruTexture.h"
#include"PuruGameSystem.h"
#include"GraphicsClass.h"


puruTexture::puruTexture() : m_Texture(nullptr), m_TextureWidth(0), m_TextureHeight(0)
{
}

puruTexture::puruTexture(WCHAR * fileName, int width, int height) : puruTexture()
{
	Initialize(fileName, width, height);
}


puruTexture::~puruTexture()
{
	Shutdown();
}

bool puruTexture::Initialize(WCHAR * fileName, int width, int height)
{
	PGE& pge = PGE::GetInstancePure();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO

	m_Texture = new TextureClass();
	if (!m_Texture)
	{
		return false;
	}

	if (!(m_Texture->Initialize(gfx->GetDevice(), fileName)))
	{
		MessageBox(NULL, L"Could not load Texture", L"Error", MB_OK);
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
		return false;
	}

	m_TextureWidth = width;
	m_TextureHeight = height;

	return true;
}

void puruTexture::Shutdown()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
}

ID3D11ShaderResourceView * puruTexture::GetTexture()
{
	if(m_Texture)
		return m_Texture->GetTexture();
	else 
		return nullptr;
}
