#pragma once

#include<windows.h>
#include"D3DClass.h"
#include"CameraClass.h"

class TextureShaderClass;
class ColorShaderClass;
class FontShaderClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	bool Render();

	void BeginScene(float red, float green, float blue, float a);
	void EndScene();

	TextureShaderClass* GetTextureShader();
	ColorShaderClass* GetColorShader();
	FontShaderClass* GetFontShader();

	void GetProjectionMatrix(D3DXMATRIX& matrix);
	void GetWorldMatrix(D3DXMATRIX& matrix);
	void GetOrthoMatrix(D3DXMATRIX& matrix);
	void GetViewMatrix(D3DXMATRIX& matrix);
	void GetBaseViewMatrix(D3DXMATRIX& matrix);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextureShaderClass* m_TextureShader;
	ColorShaderClass* m_ColorShader;
	FontShaderClass* m_FontShader;

	D3DXMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_orthoMatrix;
};

