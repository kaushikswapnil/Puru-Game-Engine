#pragma once

//This has to be defined or compiler generates warning messages
#define DIRECTINPUT_VERSION 0x0800

//Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include<dinput.h>	

class InputClass
{
public:
	struct MouseState 
	{
		int m_mouseX, m_mouseY;
		bool pin, pinstate;
		DIMOUSESTATE m_mouseState;
	};
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);

	bool IsKeyDown(unsigned int key);
	bool IsKeyUp(unsigned int key);

	bool IsMouseLButtonDown();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	//The first three are interfaces to DInput, Keyboard device, and Mouse device
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	//Recording the current state of keyboard and mouse
	unsigned char m_keyboardState[256];
	MouseState m_mouseInfo;

	int m_screenWidth, m_screenHeight;

};

