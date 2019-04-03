#include "GraphicsClass.h"
#include"TextureShaderClass.h"
#include"ColorShaderClass.h"
#include"FontShaderClass.h"
#include"Globals.h"

GraphicsClass::GraphicsClass() : m_D3D(nullptr), m_Camera(nullptr), m_TextureShader(nullptr), m_FontShader(nullptr), m_ColorShader(nullptr)
{
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}


GraphicsClass::~GraphicsClass()
{
	Shutdown();
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	//Create the d3d object
	m_D3D = new D3DClass();
	if (!m_D3D)
	{
		return false;
	}

	//Initialize the Direct3D Object
	result = m_D3D->Initialize(screenWidth, screenHeight, GraphicsGlobals::VSYNC_ENABLED, hwnd, GraphicsGlobals::FULL_SCREEN, GraphicsGlobals::SCREEN_DEPTH, GraphicsGlobals::SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create new camera
	m_Camera = new CameraClass();
	if (!m_Camera)
	{
		return false;
	}

	//Initialize a base view matrix with the camera for 2D UI rendering
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	//Set initial position of camera
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_TextureShader = new TextureShaderClass();
	if (!m_TextureShader)
	{
		return false;
	}

	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize TextureShader Object", L"Error", MB_OK);
		return false;
	}

	m_FontShader = new FontShaderClass();
	if (!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize FontShader Object", L"Error", MB_OK);
		return false;
	}

	m_ColorShader = new ColorShaderClass();
	if (!m_FontShader)
	{
		return false;
	}

	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader Object", L"Error", MB_OK);
		return false;
	}


	return true;
}

void GraphicsClass::Shutdown()
{

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;;
		m_TextureShader = nullptr;
	}
	
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;;
		m_FontShader = nullptr;
	}

	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;;
		m_ColorShader = nullptr;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_D3D)
	{
		delete m_D3D;
		m_D3D = nullptr;
	}
}

ID3D11Device * GraphicsClass::GetDevice()
{
	return m_D3D->GetDevice();
}

ID3D11DeviceContext * GraphicsClass::GetDeviceContext()
{
	return m_D3D->GetDeviceContext();
}

bool GraphicsClass::Render()
{
	
	//bool result;

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	////Put model vertex and index buffers on graphics pipeline to prepare for drawing
	

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::BeginScene(float red, float green, float blue, float a)
{
	m_D3D->BeginScene(red, green, blue, a);

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and D3D objects.
	m_Camera->GetViewMatrix(m_viewMatrix);
	m_D3D->GetWorldMatrix(m_worldMatrix);
	m_D3D->GetProjectionMatrix(m_projectionMatrix);
	m_D3D->GetOrthoMatrix(m_orthoMatrix);

	TurnZBufferOff();
	TurnOnAlphaBlending();

}

void GraphicsClass::EndScene()
{
	TurnOffAlphaBlending();
	TurnZBufferOn();
	m_D3D->EndScene();
}

TextureShaderClass* GraphicsClass::GetTextureShader()
{
	return m_TextureShader;
}

ColorShaderClass* GraphicsClass::GetColorShader()
{
	return m_ColorShader;
}

FontShaderClass* GraphicsClass::GetFontShader()
{
	return m_FontShader;
}

void GraphicsClass::GetProjectionMatrix(D3DXMATRIX & matrix)
{
	m_D3D->GetProjectionMatrix(matrix);
}

void GraphicsClass::GetWorldMatrix(D3DXMATRIX & matrix)
{
	m_D3D->GetWorldMatrix(matrix);
}

void GraphicsClass::GetOrthoMatrix(D3DXMATRIX & matrix)
{
	m_D3D->GetOrthoMatrix(matrix);
}

void GraphicsClass::GetViewMatrix(D3DXMATRIX & matrix)
{
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(matrix);
}

void GraphicsClass::GetBaseViewMatrix(D3DXMATRIX & matrix)
{
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(matrix);
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
}

void GraphicsClass::TurnZBufferOn()
{
	m_D3D->TurnZBufferOn();
}

void GraphicsClass::TurnZBufferOff()
{
	m_D3D->TurnZBufferOff();
}

void GraphicsClass::TurnOnAlphaBlending()
{
	m_D3D->TurnOnAlphaBlending();
}

void GraphicsClass::TurnOffAlphaBlending()
{
	m_D3D->TurnOffAlphaBlending();
}
