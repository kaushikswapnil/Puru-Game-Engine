#include "puruGUIText.h"
#include "puruFont.h"


puruGUIText::puruGUIText() : puruGUIObject(), m_text(nullptr)
{
}

puruGUIText::puruGUIText(int _id, int _x, int _y, char * text, puruFont * _fnt) : puruGUIObject(), m_text(nullptr)
{
	Initialize(_id, _x, _y, text, _fnt);
}

puruGUIText::puruGUIText(int _id, int _x, int _y, const char * text, puruFont * _fnt) : puruGUIObject(), m_text(nullptr)
{
	Initialize(_id, _x, _y, text, _fnt);
}


void puruGUIText::Render()
{
	fnt->printf(rect.x1, rect.y1, m_text);
}

void puruGUIText::Update(float dt)
{
}

void puruGUIText::Enter()
{
}

void puruGUIText::Leave()
{
}

bool puruGUIText::TestPoint(float x, float y)
{
	return rect.TestPoint(x, y);
}

puruGUIText::~puruGUIText()
{
	fnt = nullptr;
}

bool puruGUIText::Initialize(int _id, int _x, int _y, char * text, puruFont * _fnt)
{
	fnt = _fnt;
	m_text = text;

	float w = 0.0f, h = 0.0f;
	fnt->GetStringDimensions(text, w, h);

	rect.Set(_x - w / 2, _y, _x + w / 2, _y + h);

	return true;
}

bool puruGUIText::Initialize(int _id, int _x, int _y, const char* text, puruFont* _fnt)
{
	char* cText = const_cast<char*>(text);
	return Initialize(_id, _x, _y, cText, _fnt);
}