#pragma once

#include "puruGUIObject.h"

class puruFont;

class puruGUIText : public puruGUIObject
{
public:
	puruGUIText();
	puruGUIText(int _id, int _x, int _y, char* text, puruFont* _fnt);
	puruGUIText(int _id, int _x, int _y, const char * text, puruFont * _fnt);
	~puruGUIText() override;

	bool Initialize(int _id, int _x, int _y, char* text, puruFont* _fnt);
	bool Initialize(int _id, int _x, int _y, const char* text, puruFont* _fnt);

	virtual void Render() override;
	virtual void Update(float dt) override;
	virtual void Enter() override;
	virtual void Leave() override;
	virtual bool TestPoint(float x, float y) override;

private:
	char* m_text;
	puruFont* fnt;
};

