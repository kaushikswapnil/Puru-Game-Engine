#pragma once

//This class holds all the states of the system

enum puruCallBackState {
	PURU_FRAME,
	PURU_RENDER
};

enum puruBoolState {
	PURU_WINDOWED,
};

enum puruIntState {
	PURU_SCREENHEIGHT,
	PURU_SCREENWIDTH,
	PURU_FPS
};

typedef bool(*puruBoolCallback)();

class SystemState
{
public:
	SystemState();
	~SystemState();

	void SetState(puruCallBackState state, puruBoolCallback value);
	void SetState(puruBoolState state, bool value);
	void SetState(puruIntState state, int value);

	bool SystemReady();

	puruBoolCallback m_frameFunc;
	puruBoolCallback m_renderFunc;

};

