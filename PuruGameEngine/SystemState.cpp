#include "SystemState.h"
#include"Globals.h"


SystemState::SystemState() : m_frameFunc(nullptr), m_renderFunc(nullptr)
{
}


SystemState::~SystemState()
{
}


void SystemState::SetState(puruBoolState state, bool value)
{
	GraphicsGlobals::FULL_SCREEN = !value;
}

void SystemState::SetState(puruIntState state, int value)
{
	switch (state)
	{
	case puruIntState::PURU_SCREENWIDTH:
		GraphicsGlobals::SCREEN_WIDTH = value;
		break;
	case puruIntState::PURU_SCREENHEIGHT:
		GraphicsGlobals::SCREEN_HEIGHT = value;
		break;
	case puruIntState::PURU_FPS:
		if (value > 0 && value < 200) {
			GraphicsGlobals::FPS = value;
			GraphicsGlobals::dtFrame = 1.0f / (float)GraphicsGlobals::FPS;
		}
		break;
	default:
		break;
	}
}

bool SystemState::SystemReady()
{
	if (!m_frameFunc)
	{
		return false;
	}

	if (!m_renderFunc)
	{
		return false;
	}

	return true;
}
