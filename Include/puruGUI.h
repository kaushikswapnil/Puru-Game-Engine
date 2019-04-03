#pragma once
#include <map>

class puruGUIObject;
class PuruGameSystem;
class puruSprite;

class puruGUI
{
public:
	puruGUI();
	~puruGUI();

	void Initialize(PuruGameSystem* _pge);
	void Shutdown();

	int Update(float dt);
	void Render();

	void Enter();
	void Leave();

	int AddObject(int id, puruGUIObject* guiObj);
	int AddObject(puruGUIObject* guiObj);
	bool DelObject(int id);

	void FlushObjList();

	void SetFocus(int id);

	void SetMouse(puruSprite* spr);

private:
	void ProcessInput();
	void ProcessDownArrow();
	void ProcessUpArrow();
	void ProcessEnterKey();
	void ProcessMouseClick();
	void ProcessMouseMovement();

	void ProcessButtonClick(std::map<int, puruGUIObject*>::iterator itGuiObj);

private:
	int m_curObj, m_prevObj, m_nObj;
	std::map<int, puruGUIObject*> m_guiObjectList;
	bool m_bObjectPressed;

	PuruGameSystem* m_pge;
	puruSprite* m_sprMouse;
	int m_mouseX, m_mouseY;
};

