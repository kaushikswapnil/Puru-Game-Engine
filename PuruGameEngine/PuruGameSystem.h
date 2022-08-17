#pragma once

#define WIN32_LEAN_AND_MEAN //This excludes certain lesser used APIs
#include<windows.h>

#include"Singleton.h"
#include"SystemState.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FpsClass.h"
#include "CpuClass.h"
#include "TimerClass.h"
#include "SmartEnum.h"

class GraphicsClass;
class InputClass;
class SoundClass;
class FpsClass;
class TimerClass;
class CpuClass;

class PuruGameSystem : public Singleton<PuruGameSystem>
{
public:
	bool Initialize();
	void Shutdown();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	//System functions
	void System_SetState(puruCallBackState state, puruBoolCallback value);
	void System_SetState(puruBoolState state, bool value) { m_SystemState.SetState(state, value); }
	void System_SetState(puruIntState state, int value) { m_SystemState.SetState(state, value); }

	//Graphics functions	
	GraphicsClass* GetGraphics() { return &m_Graphics; }

	void Gfx_BeginScene(float red, float green = 0.0f, float blue = 0.0f, float a = 1.0f);
	void Gfx_EndScene();

	int Gfx_GetScreenWidth();
	int Gfx_GetScreenHeight();

	//Input functions
	bool Input_IsKeyDown(int key);
	bool Input_IsKeyUp(int key);
	bool Input_IsMouseLButtonDown();
	void Input_GetMousePosition(int& x, int&y);

	//Sound Functions
	SoundClass* GetSound();

	//Timer and Performance functions
	float Timer_GetDelta();
	int Fps_GetFPS();
	int Cpu_GetCpuUsage();

	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_HInstance;
	HWND m_Hwnd;

	//Framework Classes
	//#TODO replace all with references to non copyable classes
	InputClass m_Input{};
	GraphicsClass m_Graphics{};
	SoundClass m_Sound{};

	//Timer and Performance classes
	FpsClass m_Fps{};
	CpuClass m_Cpu{};
	TimerClass m_Timer{};

	//State handling classes
	SystemState m_SystemState{};

	float m_dtFrameCallTime;

};

typedef PuruGameSystem PGE;

//The WndProc and ApplicationHandle are included so that the built in windows system messaging can be redirected to our own MessageHandler
//Function Prototypes

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Globals
static PuruGameSystem* ApplicationHandle = nullptr;

/****************************************************************************
*
*      PGE keyboard scan codes
*
****************************************************************************/
#define PGEK_ESCAPE          0x01
#define PGEK_1               0x02
#define PGEK_2               0x03
#define PGEK_3               0x04
#define PGEK_4               0x05
#define PGEK_5               0x06
#define PGEK_6               0x07
#define PGEK_7               0x08
#define PGEK_8               0x09
#define PGEK_9               0x0A
#define PGEK_0               0x0B
#define PGEK_MINUS           0x0C    /* - on main keyboard */
#define PGEK_EQUALS          0x0D
#define PGEK_BACKSPACE            0x0E    /* backspace */
#define PGEK_TAB             0x0F
#define PGEK_Q               0x10
#define PGEK_W               0x11
#define PGEK_E               0x12
#define PGEK_R               0x13
#define PGEK_T               0x14
#define PGEK_Y               0x15
#define PGEK_U               0x16
#define PGEK_I               0x17
#define PGEK_O               0x18
#define PGEK_P               0x19
#define PGEK_LBRACKET        0x1A
#define PGEK_RBRACKET        0x1B
#define PGEK_RETURN          0x1C    /* Enter on main keyboard */
#define PGEK_LCONTROL        0x1D
#define PGEK_A               0x1E
#define PGEK_S               0x1F
#define PGEK_D               0x20
#define PGEK_F               0x21
#define PGEK_G               0x22
#define PGEK_H               0x23
#define PGEK_J               0x24
#define PGEK_K               0x25
#define PGEK_L               0x26
#define PGEK_SEMICOLON       0x27
#define PGEK_APOSTROPHE      0x28
#define PGEK_GRAVE           0x29    /* accent grave */
#define PGEK_LSHIFT          0x2A
#define PGEK_BACKSLASH       0x2B
#define PGEK_Z               0x2C
#define PGEK_X               0x2D
#define PGEK_C               0x2E
#define PGEK_V               0x2F
#define PGEK_B               0x30
#define PGEK_N               0x31
#define PGEK_M               0x32
#define PGEK_COMMA           0x33
#define PGEK_PERIOD          0x34    /* . on main keyboard */
#define PGEK_SLASH           0x35    /* / on main keyboard */
#define PGEK_RSHIFT          0x36
#define PGEK_NUMPADSTAR        0x37    /* * on numeric keypad */
#define PGEK_LALT           0x38    /* left Alt */
#define PGEK_SPACE           0x39
#define PGEK_CAPSLOCK         0x3A
#define PGEK_F1              0x3B
#define PGEK_F2              0x3C
#define PGEK_F3              0x3D
#define PGEK_F4              0x3E
#define PGEK_F5              0x3F
#define PGEK_F6              0x40
#define PGEK_F7              0x41
#define PGEK_F8              0x42
#define PGEK_F9              0x43
#define PGEK_F10             0x44
#define PGEK_NUMLOCK         0x45
#define PGEK_SCROLL          0x46    /* Scroll Lock */
#define PGEK_NUMPAD7         0x47
#define PGEK_NUMPAD8         0x48
#define PGEK_NUMPAD9         0x49
#define PGEK_NUMPADMINUS        0x4A    /* - on numeric keypad */
#define PGEK_NUMPAD4         0x4B
#define PGEK_NUMPAD5         0x4C
#define PGEK_NUMPAD6         0x4D
#define PGEK_NUMPADPLUS             0x4E    /* + on numeric keypad */
#define PGEK_NUMPAD1         0x4F
#define PGEK_NUMPAD2         0x50
#define PGEK_NUMPAD3         0x51
#define PGEK_NUMPAD0         0x52
#define PGEK_NUMPADPERIOD         0x53    /* . on numeric keypad */
#define PGEK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define PGEK_F11             0x57
#define PGEK_F12             0x58
#define PGEK_F13             0x64    /*                     (NEC PC98) */
#define PGEK_F14             0x65    /*                     (NEC PC98) */
#define PGEK_F15             0x66    /*                     (NEC PC98) */
#define PGEK_KANA            0x70    /* (Japanese keyboard)            */
#define PGEK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define PGEK_CONVERT         0x79    /* (Japanese keyboard)            */
#define PGEK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define PGEK_YEN             0x7D    /* (Japanese keyboard)            */
#define PGEK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define PGEK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define PGEK_PREVTRACK       0x90    /* Previous Track (PGEK_CIRCUMFLEX on Japanese keyboard) */
#define PGEK_AT              0x91    /*                     (NEC PC98) */
#define PGEK_COLON           0x92    /*                     (NEC PC98) */
#define PGEK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define PGEK_KANJI           0x94    /* (Japanese keyboard)            */
#define PGEK_STOP            0x95    /*                     (NEC PC98) */
#define PGEK_AX              0x96    /*                     (Japan AX) */
#define PGEK_UNLABELED       0x97    /*                        (J3100) */
#define PGEK_NEXTTRACK       0x99    /* Next Track */
#define PGEK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define PGEK_RCONTROL        0x9D
#define PGEK_MUTE            0xA0    /* Mute */
#define PGEK_CALCULATOR      0xA1    /* Calculator */
#define PGEK_PLAYPAUSE       0xA2    /* Play / Pause */
#define PGEK_MEDIASTOP       0xA4    /* Media Stop */
#define PGEK_VOLUMEDOWN      0xAE    /* Volume - */
#define PGEK_VOLUMEUP        0xB0    /* Volume + */
#define PGEK_WEBHOME         0xB2    /* Web home */
#define PGEK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define PGEK_NUMPADSLASH          0xB5    /* / on numeric keypad */
#define PGEK_SYSRQ           0xB7
#define PGEK_RALT           0xB8    /* right Alt */
#define PGEK_PAUSE           0xC5    /* Pause */
#define PGEK_HOME            0xC7    /* Home on arrow keypad */
#define PGEK_UPARROW              0xC8    /* UpArrow on arrow keypad */
#define PGEK_PGUP           0xC9    /* PgUp on arrow keypad */
#define PGEK_LEFTARROW            0xCB    /* LeftArrow on arrow keypad */
#define PGEK_RIGHTARROW           0xCD    /* RightArrow on arrow keypad */
#define PGEK_END             0xCF    /* End on arrow keypad */
#define PGEK_DOWNARROW            0xD0    /* DownArrow on arrow keypad */
#define PGEK_PGDN            0xD1    /* PgDn on arrow keypad */
#define PGEK_INSERT          0xD2    /* Insert on arrow keypad */
#define PGEK_DELETE          0xD3    /* Delete on arrow keypad */
#define PGEK_LWIN            0xDB    /* Left Windows key */
#define PGEK_RWIN            0xDC    /* Right Windows key */
#define PGEK_APPS            0xDD    /* AppMenu key */
#define PGEK_POWER           0xDE    /* System Power */
#define PGEK_SLEEP           0xDF    /* System Sleep */
#define PGEK_WAKE            0xE3    /* System Wake */
#define PGEK_WEBSEARCH       0xE5    /* Web Search */
#define PGEK_WEBFAVORITES    0xE6    /* Web Favorites */
#define PGEK_WEBREFRESH      0xE7    /* Web Refresh */
#define PGEK_WEBSTOP         0xE8    /* Web Stop */
#define PGEK_WEBFORWARD      0xE9    /* Web Forward */
#define PGEK_WEBBACK         0xEA    /* Web Back */
#define PGEK_MYCOMPUTER      0xEB    /* My Computer */
#define PGEK_MAIL            0xEC    /* Mail */
#define PGEK_MEDIASELECT     0xED    /* Media Select */

