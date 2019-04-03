#include "puruGUI.h"
#include "puruGUIObject.h"
#include "PuruGameSystem.h"
#include "puruSprite.h"

puruGUI::puruGUI() : m_nObj(0), m_curObj(-1), m_prevObj(-1), m_bObjectPressed(false), m_sprMouse(nullptr), m_mouseX(-1), m_mouseY(-1), m_guiObjectList({})
{
}


puruGUI::~puruGUI()
{
	Shutdown();
}

void puruGUI::Initialize(PuruGameSystem* _pge)
{
	m_pge = _pge;
}

void puruGUI::Shutdown()
{
	FlushObjList();
	m_sprMouse = nullptr;
}

int puruGUI::Update(float dt)
{
	if (m_curObj != -1)
	{		
		m_guiObjectList[m_curObj]->Update(dt);

		ProcessInput();

		if (m_bObjectPressed == true && m_curObj == m_prevObj)
		{
			m_bObjectPressed = false;
			return -1;
		}
		else if (m_curObj != m_prevObj)
		{
			m_prevObj = m_curObj;
			return m_curObj;
		}
	}

	return m_curObj;
	
}

void puruGUI::Render()
{
	std::map<int, puruGUIObject*>::iterator itGuiObj = m_guiObjectList.begin();
	while (itGuiObj != m_guiObjectList.end())
	{
		(itGuiObj->second)->Render();
		++itGuiObj;
	}
	if (m_sprMouse)
	{
		m_sprMouse->Render(m_mouseX, m_mouseY);
	}
}

void puruGUI::Enter()
{
	m_bObjectPressed = false;
}

void puruGUI::ProcessInput()
{
	ProcessMouseMovement();

	if (m_pge->Input_IsKeyDown(PGEK_DOWNARROW))
	{
		ProcessDownArrow();
	}
	else if (m_pge->Input_IsKeyDown(PGEK_UPARROW))
	{
		ProcessUpArrow();
	}
	else if (m_pge->Input_IsKeyDown(PGEK_RETURN))
	{
		ProcessEnterKey();
	}
	else if (m_pge->Input_IsMouseLButtonDown())
	{
		ProcessMouseClick();
	}
}

void puruGUI::ProcessDownArrow()
{
	std::map<int, puruGUIObject*>::iterator itGUIObj = m_guiObjectList.begin();
	itGUIObj = m_guiObjectList.find(m_curObj);
	if (itGUIObj == m_guiObjectList.end())
	{
		return;
	}
	itGUIObj++;
	if (itGUIObj == m_guiObjectList.end())
	{
		itGUIObj = m_guiObjectList.begin();
	}

	m_prevObj = m_curObj;
	m_curObj = itGUIObj->first;
	SetFocus(m_curObj);
}

void puruGUI::ProcessUpArrow()
{
	std::map<int, puruGUIObject*>::iterator itGUIObj = m_guiObjectList.begin();
	itGUIObj = m_guiObjectList.find(m_curObj);
	if (itGUIObj == m_guiObjectList.end())
	{
		return;
	}
	if (itGUIObj == m_guiObjectList.begin())
	{
		return;
	}
	else
	{
		itGUIObj--;
	}

	m_prevObj = m_curObj;
	m_curObj = itGUIObj->first;
	SetFocus(m_curObj);
}

void puruGUI::ProcessEnterKey()
{
	if (m_curObj == -1)
	{
		return;
	}
	
	std::map<int, puruGUIObject*>::iterator itGUIObj = m_guiObjectList.begin();
	itGUIObj = m_guiObjectList.find(m_curObj);
	if (itGUIObj == m_guiObjectList.end())
	{
		return;
	}
	
	ProcessButtonClick(itGUIObj);
}

void puruGUI::ProcessMouseClick()
{
	if (m_sprMouse)
	{
		std::map<int, puruGUIObject*>::iterator itGuiObj = m_guiObjectList.begin();
		while (itGuiObj != m_guiObjectList.end())
		{
			if ((itGuiObj->second)->TestPoint(m_mouseX, m_mouseY))
			{
				ProcessButtonClick(itGuiObj);
				break;
			}
			itGuiObj++;
		}
	}
}

void puruGUI::ProcessMouseMovement()
{
	if(m_sprMouse)
	{
		m_pge->Input_GetMousePosition(m_mouseX, m_mouseY);
		std::map<int, puruGUIObject*>::iterator itGuiObj = m_guiObjectList.begin();
		while (itGuiObj != m_guiObjectList.end())
		{
			if ((itGuiObj->second)->TestPoint(m_mouseX, m_mouseY))
			{
				m_prevObj = m_curObj;
				m_curObj = itGuiObj->first;
				SetFocus(itGuiObj->first);
				break;
			}
			itGuiObj++;
		}
	}
}

void puruGUI::ProcessButtonClick(std::map<int, puruGUIObject*>::iterator itGuiObj)
{
	m_prevObj = m_curObj;
	m_curObj = itGuiObj->first;
	m_bObjectPressed = true;
	SetFocus(itGuiObj->first);
	return;
}

void puruGUI::Leave()
{
}

int puruGUI::AddObject(int id, puruGUIObject * guiObj)
{
	if (m_guiObjectList.find(id) != m_guiObjectList.end())
	{
		return -1;
	}
	
	m_guiObjectList[id] = guiObj;
	m_nObj++;

	return id;
}

int puruGUI::AddObject(puruGUIObject * guiObj)
{
	int id = guiObj->GetID();

	if (m_guiObjectList.find(id) != m_guiObjectList.end())
	{
		id = m_guiObjectList.size();
		guiObj->SetID(id);
	}

	m_guiObjectList[id] = guiObj;
	m_nObj++;

	return id;
}

bool puruGUI::DelObject(int id)
{
	if (m_guiObjectList.find(id) != m_guiObjectList.end())
	{
		delete m_guiObjectList[id];
		m_guiObjectList.erase(id);
		m_nObj--;
	}
	else 
	{
		return false;
	}
	return true;
}

void puruGUI::FlushObjList()
{
	std::map<int, puruGUIObject*>::iterator itGuiObj = m_guiObjectList.begin();
	while (itGuiObj != m_guiObjectList.end())
	{
		delete itGuiObj->second;
		itGuiObj = m_guiObjectList.erase(itGuiObj);
	}
}

void puruGUI::SetFocus(int id)
{
	if (m_guiObjectList.find(id) == m_guiObjectList.end())
	{
		return;
	}

	if (m_curObj == -1)
	{
		m_curObj = id;
	}
	if (m_prevObj != m_curObj)
	{
		if (m_prevObj != -1)
		{
			m_guiObjectList[m_prevObj]->Leave();
		}
		m_prevObj = m_curObj;
		m_curObj = id;
		m_guiObjectList[m_curObj]->Enter();
	}

}

void puruGUI::SetMouse(puruSprite * spr)
{
	m_sprMouse = spr;
}
