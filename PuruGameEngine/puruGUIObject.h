#pragma once
#include<string>
#include "puruRect.h"

class puruFont;
class puruSound;

class puruGUIObject
{
public:
	struct guiObjectColor {
		float red, green, blue;
	};
public:
	puruGUIObject();
	virtual ~puruGUIObject();

	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual bool TestPoint(float x, float y) = 0;

	virtual int GetID() { return id; }
	virtual void SetID(int _id) { id = _id; }

protected:
	int id;
	bool bFocused;
	puruRect rect;
};

