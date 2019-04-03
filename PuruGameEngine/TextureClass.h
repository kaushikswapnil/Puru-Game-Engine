#pragma once

//The texture class encapsulates loading, unloading, and accessing of a single texture resource

#include<d3d11.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device *device, WCHAR *fileName);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

