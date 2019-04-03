#include "PuruGameSystem.h"
#include"InputClass.h"
#include"GraphicsClass.h"
#include"SoundClass.h"
#include"FpsClass.h"
#include"CpuClass.h"
#include"TimerClass.h"
#include"Globals.h"

PuruGameSystem* PuruGameSystem::m_pgeInstance;

PuruGameSystem::PuruGameSystem() : Singleton(), m_Input(nullptr), m_Graphics(nullptr), m_dtFrameCallTime(0.0f)
{
}

PuruGameSystem * PuruGameSystem::GetInstance()
{
	if (!m_pgeInstance)
	{
		m_cs.Lock();
		if (!m_pgeInstance)
		{
			m_pgeInstance = new PuruGameSystem();
		}
		m_cs.Unlock();
	}
	return m_pgeInstance;
}

PuruGameSystem::~PuruGameSystem()
{
}

bool PuruGameSystem::Initialize()
{
	if (!m_SystemState.SystemReady())
	{
		return false;
	}

	int screenWidth, screenHeight;
	bool result;

	screenWidth = GraphicsGlobals::SCREEN_WIDTH;
	screenHeight = GraphicsGlobals::SCREEN_HEIGHT;

	//initialize the old windows API
	InitializeWindows(screenWidth, screenHeight);

	//Create the input object. This object will be used to handle reading the keyboard input from the user
	m_Input = new InputClass();
	if (!m_Input)
	{
		return false;
	}

	//Initialize input object
	m_Input->Initialize(m_hInstance, m_hwnd, screenWidth, screenHeight);

	//Create the graphics object to handle all rendering
	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
	{
		return false;
	}

	//initialize the graphics object
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Sound = new SoundClass();
	if (!m_Sound)
	{
		return false;
	}

	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass();
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass();
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the timer object.
	m_Timer = new TimerClass();
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_dtFrameCallTime = GraphicsGlobals::dtFrame;

	return true;
}

void PuruGameSystem::Shutdown()
{
	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = nullptr;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = nullptr;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = nullptr;
	}

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	//Shutdown windows
	ShutdownWindows();

	DestroyInstance();
}

void PuruGameSystem::DestroyInstance()
{
	if (m_pgeInstance)
	{
		delete m_pgeInstance;
		m_pgeInstance = nullptr;
	}
}

void PuruGameSystem::Run()
{
	MSG msg;
	bool done = false, result;

	//Initialize message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message
	while (!done)
	{
		//handle windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signals to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			m_Timer->OutsideFrame();
			//Frame processing
			if (GraphicsGlobals::FPS > 0) //Only process frame during frame time
			{
				m_dtFrameCallTime -= m_Timer->GetLastCallTime();
				if (m_dtFrameCallTime > 0.0f)
				{
					continue;
				}
				m_dtFrameCallTime = m_dtFrameCallTime = GraphicsGlobals::dtFrame;
			}
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT PuruGameSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void PuruGameSystem::Gfx_BeginScene(float red, float green, float blue, float a)
{
	m_Graphics->BeginScene(red, green, blue, a);
}

void PuruGameSystem::Gfx_EndScene()
{
	m_Graphics->EndScene();
}

int PuruGameSystem::Gfx_GetScreenWidth()
{
	return GraphicsGlobals::SCREEN_WIDTH;
}

int PuruGameSystem::Gfx_GetScreenHeight()
{
	return GraphicsGlobals::SCREEN_HEIGHT;
}

bool PuruGameSystem::Input_IsKeyDown(int key)
{
	return m_Input->IsKeyDown(key);
}

bool PuruGameSystem::Input_IsKeyUp(int key)
{
	return m_Input->IsKeyUp(key);
}

bool PuruGameSystem::Input_IsMouseLButtonDown()
{
	return m_Input->IsMouseLButtonDown();
}

void PuruGameSystem::Input_GetMousePosition(int & x, int & y)
{
	m_Input->GetMouseLocation(x, y);
}

SoundClass * PuruGameSystem::GetSound()
{
	return m_Sound;
}

float PuruGameSystem::Timer_GetDelta()
{
	return m_Timer->GetTime();
}

int PuruGameSystem::Fps_GetFPS()
{
	return m_Fps->GetFPS();
}

int PuruGameSystem::Cpu_GetCpuUsage()
{
	return m_Cpu->GetCpuPercentage();
}

bool PuruGameSystem::Frame()
{
	bool result;

	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	result = m_SystemState.m_frameFunc();
	if (!result)
	{
		return false;
	}
		
	result = m_SystemState.m_renderFunc();
	if (!result)
	{
		return false;
	}

	//Do frame processing for Input
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void PuruGameSystem::InitializeWindows(int & screenWidth, int & screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get external pointer to this object
	ApplicationHandle = this;

	//Get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	//Give the Application a name
	m_applicationName = L"DX Practice Puru";

	//Setup the windows class with defaults
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register window class
	RegisterClassEx(&wc);

	//Determine the resolution of clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (GraphicsGlobals::FULL_SCREEN)
	{
		//if full screen set the size to max size of users desktop
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set position of window to top left corner
		posY = posX = 0;
	}
	else
	{
		//If windows set it to 800x600
		screenWidth = GraphicsGlobals::SCREEN_WIDTH;
		screenHeight = GraphicsGlobals::SCREEN_HEIGHT;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Create the window with screen settings and get its handle
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	//Bring window up to screen and set as focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide mouse 
	ShowCursor(false);
}

void PuruGameSystem::ShutdownWindows()
{
	//Show mouse cursor
	ShowCursor(true);

	//Fix display settings if leaving full screen
	if (GraphicsGlobals::FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//Remove the application instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	//Release the pointer to this class
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
		
		//All other messages are passed onto Systems message handler
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		break;
	}
}
