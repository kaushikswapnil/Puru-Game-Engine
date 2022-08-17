#include "puruQuad.h"
#include "PuruGameSystem.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "TextureShaderClass.h"
#include "Globals.h"
#include <D3DX11.h>

puruQuad::puruQuad() : m_Bitmap(nullptr), m_TextureShader(nullptr)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		v[i].x = 0;
		v[i].y = 0;
	}
}

puruQuad::puruQuad(puruTexture * texture, float texLeft, float texTop, unsigned int width, unsigned int height)
{
	Initialize(texture, texLeft, texTop, width, height);
}

puruQuad::puruQuad(const puruQuad &other)
{
	PGE& pge = PGE::GetInstancePure();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO
	
	m_Bitmap = new BitmapClass();
	*m_Bitmap = *(other.m_Bitmap);

	m_texLeft = other.m_texLeft;
	m_texTop = other.m_texTop;

	for (unsigned int i = 0; i < 4; i++)
	{
		v[i].x = other.v[i].x;
		v[i].y = other.v[i].y;
	}

	m_Bitmap->CalculateTextureLocations(m_texLeft, m_texTop);
	m_TextureShader = gfx->GetTextureShader();
}


puruQuad::~puruQuad()
{
	Shutdown();
}

puruQuad & puruQuad::operator=(const puruQuad & other)
{
	PGE& pge = PGE::GetInstancePure();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO

	m_texLeft = other.m_texLeft;
	m_texTop = other.m_texTop;
	
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		m_Bitmap = nullptr;
	}
	
	m_Bitmap = new BitmapClass();
	*m_Bitmap = *(other.m_Bitmap);

	for (unsigned int i = 0; i < 4; i++)
	{
		v[i].x = other.v[i].x;
		v[i].y = other.v[i].y;
	}

	m_Bitmap->CalculateTextureLocations(m_texLeft, m_texTop);
	m_TextureShader = gfx->GetTextureShader();

	return *this;
}

bool puruQuad::Initialize(puruTexture * texture, float texLeft, float texTop, unsigned int width, unsigned int height)
{
	PGE& pge = PGE::GetInstancePure();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO

	m_texLeft = texLeft;
	m_texTop = texTop;

	m_Bitmap = new BitmapClass();

	if (!(m_Bitmap->Initialize(gfx->GetDevice(), pge.Gfx_GetScreenWidth(), pge.Gfx_GetScreenHeight(), texture, width, height)))
	{
		delete m_Bitmap;
		m_Bitmap = nullptr;
		return false;
	}
	CalculateVerticeLocations(GraphicsGlobals::SCREEN_WIDTH / 2, GraphicsGlobals::SCREEN_HEIGHT / 2);
	m_Bitmap->CalculateTextureLocations(texLeft, texTop);
	m_TextureShader = gfx->GetTextureShader();

	return true;
}

void puruQuad::CalculateVerticeLocations(int x, int y)
{
	unsigned int width, height;
	width = m_Bitmap->GetWidth();
	height = m_Bitmap->GetHeight();

	v[0].x = x - (width / 2);
	v[0].y = y - (height / 2);

	v[1].x = x + (width / 2);
	v[1].y = y - (height / 2);

	v[2].x = x + (width / 2);
	v[2].y = y + (height / 2);

	v[3].x = x - (width / 2);
	v[3].y = y + (height / 2);
}

void puruQuad::Shutdown()
{
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		m_Bitmap = nullptr;
	}
	m_TextureShader = nullptr;
}

void puruQuad::Render(int x, int y, float scale)
{
	CalculateVerticeLocations(x, y);
	Render(scale);
}

void puruQuad::RenderQuad()
{
	Render();
}

void puruQuad::SetTextureLocations(float texLeft, float texTop)
{
	m_texLeft = texLeft;
	m_texTop = texTop;

	if (m_texLeft < 0.0f)
	{
		m_texLeft = 0.0f;
	}

	if (m_texTop < 0.0f)
	{
		m_texTop = 0.0f;
	}

	m_Bitmap->CalculateTextureLocations(m_texLeft, m_texTop);
}

void puruQuad::Render(float scale)
{
	PGE& pge = PGE::GetInstancePure();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;

	gfx->GetWorldMatrix(worldMatrix);
	gfx->GetViewMatrix(viewMatrix);
	gfx->GetOrthoMatrix(orthoMatrix);

	if (!m_Bitmap || !m_TextureShader)
	{
		return;
	}

	if (!m_Bitmap->Render(gfx->GetDeviceContext(), v[0].x, v[0].y, scale))
	{
		return;
	}

	//Render the model using the color shader
	if(!m_TextureShader->Render(gfx->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	{
		return;
	}

}

puruTexture* puruQuad::GetTexture()
{
	return m_Bitmap->GetPTexture();
}

void puruQuad::UpdateBitmapDimensions()
{
	unsigned int width = (unsigned int)(v[0].x - v[1].x);
	unsigned int height = (unsigned int)(v[0].y - v[3].y);

	m_Bitmap->SetBitmapDimensions(width, height);
}
