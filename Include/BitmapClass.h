#pragma once

//The bitmap class will be used to represent an individual 2D image that needs 
//to be rendered to the screen. Every image needs a 2D class. This is similar to a
//model class but for 2D objects

#include<D3D11.h>
#include<D3DX10math.h>

#include"puruTexture.h"

class BitmapClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass&other);
	~BitmapClass();

	BitmapClass& operator=(const BitmapClass& other);

	bool Initialize(ID3D11Device *device, int screenWidth, int screenHeight, puruTexture* texture, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext *deviceContext, int positionX, int positionY, float scale = 1.0f);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetBitmapDimensions(unsigned int width, unsigned int height) { m_bitmapWidth = width; m_bitmapHeight = height; CalculateTextureLocations(m_texLeft, m_texTop); }

	void CalculateTextureLocations(float left, float top);

	int GetWidth() { return m_bitmapWidth; }
	void SetWidth(int val) { m_bitmapWidth = val; CalculateTextureLocations(m_texLeft, m_texTop); }

	int GetHeight() { return m_bitmapHeight; }
	void SetHeight(int val) { m_bitmapHeight = val; CalculateTextureLocations(m_texLeft, m_texTop); }

	puruTexture* GetPTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext *deviceContext, int positionX, int positionY, float scale = 1.0f);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(puruTexture* texture);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	puruTexture *m_Texture;

	int m_screenWidth, m_screenHeight;
	unsigned int m_bitmapWidth, m_bitmapHeight; //Texture dimensions
	int m_previousPosX, m_previousPosY;

	float m_texLeft, m_texRight, m_texTop, m_texBottom;
};

