#include "puruFont.h"
#include "TextClass.h"
#include "PuruGameSystem.h"
#include "GraphicsClass.h"
#include "FontShaderClass.h"

#include<D3DX10math.h>

puruFont::puruFont() : m_text(nullptr)
{
}

puruFont::puruFont(char * fontDataFilename, char * textureFilename) : m_text(nullptr)
{
	Initialize(fontDataFilename, textureFilename);
}


puruFont::~puruFont()
{
	Shutdown();
}

void puruFont::Shutdown()
{
	if (m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = nullptr;
	}
}

bool puruFont::Initialize(char * fontDataFilename, char * textureFilename)
{
	if (m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = nullptr;
	}

	PGE& pge = PGE::GetInstance();
	GraphicsClass* gfx = pge.GetGraphics(); //#TODO
	D3DXMATRIX baseViewMatrix;
	FontShaderClass* ptrFontShader = nullptr;

	m_text = new TextClass();
	if (!m_text)
	{
		return false;
	}

	gfx->GetBaseViewMatrix(baseViewMatrix);
	ptrFontShader = gfx->GetFontShader();

	if (!baseViewMatrix || !ptrFontShader)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = nullptr;
		return false;
	}
	//#TODO
	m_text->Initialize(gfx->GetDevice(), gfx->GetDeviceContext(), pge.Gfx_GetScreenWidth(), pge.Gfx_GetScreenHeight(), baseViewMatrix, ptrFontShader, fontDataFilename, textureFilename);

	return true;
}

void puruFont::printf(int x, int y, char* strToPrint, float red, float green, float blue)
{
	bool result;

	PGE& pge = PGE::GetInstance();
	GraphicsClass* gfx = pge.GetGraphics();  //#TODO
	D3DXMATRIX worldMatrix, orthomatrix;
	ID3D11DeviceContext* deviceContext;

	deviceContext = gfx->GetDeviceContext();

	result = m_text->Print(x, y, strToPrint, red, green, blue, deviceContext);
	if (!result)
	{
		gfx->TurnOnAlphaBlending();
		gfx->TurnZBufferOn();
		return;
	}

	gfx->GetWorldMatrix(worldMatrix);
	gfx->GetOrthoMatrix(orthomatrix);

	result = m_text->Render(deviceContext, worldMatrix, orthomatrix);
}

void puruFont::printf(int x, int y, const char* strToPrint, float red, float green, float blue)
{
	char* cstrToPrint = const_cast<char*>(strToPrint);

	printf(x, y, cstrToPrint, red, green, blue);
}

void puruFont::printf(int x, int y, std::string strToPrint, float red, float green, float blue)
{
	bool result;

	PGE& pge = PGE::GetInstance();
	GraphicsClass* gfx = pge.GetGraphics();  //#TODO
	D3DXMATRIX worldMatrix, orthomatrix;
	ID3D11DeviceContext* deviceContext;

	deviceContext = gfx->GetDeviceContext();

	char* cstrToPrint = (char*)strToPrint.c_str();

	result = m_text->Print(x, y, cstrToPrint, red, green, blue, deviceContext);
	if (!result)
	{
		gfx->TurnOnAlphaBlending();
		gfx->TurnZBufferOn();
		return;
	}

	gfx->GetWorldMatrix(worldMatrix);
	gfx->GetOrthoMatrix(orthomatrix);

	result = m_text->Render(deviceContext, worldMatrix, orthomatrix);

}

void puruFont::GetStringDimensions(char * strToPrint, float & width, float & height)
{
	bool result;

	PGE& pge = PuruGameSystem::GetInstance();
	GraphicsClass* gfx = pge.GetGraphics();  //#TODO

	result = m_text->Print(100, 100, strToPrint, 1.0f, 1.0f, 1.0f, gfx->GetDeviceContext());
	if (!result)
	{
		return;
	}

	width = m_text->GetWidth();
	height = m_text->GetHeight();
}
