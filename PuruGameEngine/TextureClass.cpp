#include "TextureClass.h"
#include<D3DX11tex.h>


TextureClass::TextureClass() : m_texture(nullptr)
{
}

TextureClass::TextureClass(const TextureClass &)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device *device, WCHAR *fileName)
{
	HRESULT result;

	//Load the texture in
	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Cannot load texture", L"Error", MB_OK);
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	//Release the texture resource
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_texture;
}
