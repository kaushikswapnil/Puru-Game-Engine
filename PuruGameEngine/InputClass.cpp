#include "InputClass.h"



InputClass::InputClass() : m_directInput(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
}

InputClass::InputClass(const InputClass &)
{
}


InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	//Store the screen size which will be used for positioning the mouse cursor
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Initialize the location of mouse
	m_mouseInfo.m_mouseX = m_mouseInfo.m_mouseY = 0;

	//Initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the direct input interface for keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set the data format. In this case since it is keyboard 
	//we can used the predefined data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//Setting the cooperative level of keyboard
	//We set it to DISCL_EXCLUSIVE so that only our
	//app can see when a key is pressed.
	//If we set it to non exclusive we have to constantly check
	//for when focus is lost and regained
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Once keyboard is set up, we can call acquire to finally get 
	//access to keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//now we set up mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set data format for mouse
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//We use non exclusive for mouse. We will have to check
	//each time it goes in and out of focus

	//Set the cooperative level of mouse to share with 
	//other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//acquire the mouse
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	//Release the main interface to DI
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

bool InputClass::Frame()
{
	bool result;

	//Read the current state of keyboard
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//Read mouse
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//Process changes in mouse and keyboard
	ProcessInput();

	return true;
}

void InputClass::GetMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = m_mouseInfo.m_mouseX;
	mouseY = m_mouseInfo.m_mouseY;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	if ((key < 0) || (key > 255))
	{
		return false;
	}
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsKeyUp(unsigned int key)
{
	if ((key < 0) || (key > 255))
	{
		return false;
	}
	if (!(m_keyboardState[key] & 0x80))
	{
		return true;
	}
	return false;
}

bool InputClass::IsMouseLButtonDown()
{
	return m_mouseInfo.pin;
}

//This will read the state of keyboard in the m_keyboardState. The state
//will show any keys that are currently pressed or not pressed. If it fails 
//reading the keyboard it can be for any of 5 reasons. We need to recover from
//lose of focus or if it is un-acquired
bool InputClass::ReadKeyboard()
{
	HRESULT result;

	//Read keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//if keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

//Read mouse is similar to readkeyboard, however mouse states are received relative to the
//previous position
bool InputClass::ReadMouse()
{
	HRESULT result;

	//Read the mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseInfo.m_mouseState);
	if (FAILED(m_mouse))
	{
		//If the mouse lost focus or was not acquired
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

//This function is where we deal with the changes that have happened in the IP devices
//since last frame
void InputClass::ProcessInput()
{
	//Update the location of mouse based on change
	m_mouseInfo.m_mouseX += m_mouseInfo.m_mouseState.lX;
	m_mouseInfo.m_mouseY += m_mouseInfo.m_mouseState.lY;

	//Ensure mouse location doesn't exceed the screen width or height
	if (m_mouseInfo.m_mouseX < 0) { m_mouseInfo.m_mouseX = 0; }
	if (m_mouseInfo.m_mouseY < 0) { m_mouseInfo.m_mouseY = 0; }

	if (m_mouseInfo.m_mouseX > m_screenWidth) { m_mouseInfo.m_mouseX = m_screenWidth; }
	if (m_mouseInfo.m_mouseY > m_screenHeight) { m_mouseInfo.m_mouseY = m_screenHeight; }

	m_mouseInfo.pin = false;

	//Has a single click occurred?
	if (m_mouseInfo.m_mouseState.rgbButtons[0] && !m_mouseInfo.pinstate)
	{
		m_mouseInfo.pinstate = true;
		m_mouseInfo.pin = true;
	}

	//reset 'pin' state
	if (!m_mouseInfo.m_mouseState.rgbButtons[0])
	{
		m_mouseInfo.pinstate = false;
	}
}
