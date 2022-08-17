#pragma once
#include <functional>

//This class holds all the states of the system

enum class puruCallBackState {
	PURU_FRAME,
	PURU_RENDER
};

enum class puruBoolState {
	PURU_WINDOWED,
};

enum class puruIntState {
	PURU_SCREENHEIGHT,
	PURU_SCREENWIDTH,
	PURU_FPS
};

typedef std::function<bool(void)> puruBoolCallback;

class SystemState
{
public:
	SystemState();
	~SystemState();

	void SetState(puruCallBackState state, puruBoolCallback value)
	{
		{
			switch (state)
			{
			case puruCallBackState::PURU_FRAME:
				m_frameFunc = value;
				break;

			case puruCallBackState::PURU_RENDER:
				m_renderFunc = value;
				break;

			default:
				break;
			}
		}
	}
	void SetState(puruBoolState state, bool value);
	void SetState(puruIntState state, int value);

	bool SystemReady();

	puruBoolCallback m_frameFunc;
	puruBoolCallback m_renderFunc;

};

