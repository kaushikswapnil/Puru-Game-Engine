#pragma once

//Text class handles all the 2D text drawing

#include"FontClass.h"
#include"FontShaderClass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, 
		D3DXMATRIX baseViewMatrix, FontShaderClass* fontShader, char* fontDataFilename, char* textureFilename);
	void Shutdown();

	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

	bool Print(int x, int y, char* text, float red, float green, float blue, ID3D11DeviceContext* deviceContext);

	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	SentenceType* m_sentence;

	float m_width, m_height;
};
