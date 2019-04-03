#include"puruGUIMenuItem.h"
#include "puruFont.h"
#include "puruSound.h"

puruGUIMenuItem::puruGUIMenuItem() : puruGUIObject(), fnt(nullptr), snd(nullptr), title(nullptr), offset(0.0f)
{
	id = -1;

	bFocused = false;

	color.red = 1.0f;
	color.green = 1.0f;
	color.blue = 1.0f;

	shadow.red = 0.625f;
	shadow.green = 0.625f;
	shadow.blue = 0.625f;
}

puruGUIMenuItem::puruGUIMenuItem(int _id, puruFont *_fnt, puruSound* _snd, float _x, float _y, char *_title) : puruGUIObject(),
	fnt(_fnt), snd(_snd), title(_title), offset(0.0f)
{
	id = _id;
	bFocused = false;

	color.red = 1.0f;
	color.green = 1.0f;
	color.blue = 1.0f;

	shadow.red = 0.625f;
	shadow.green = 0.625f;
	shadow.blue = 0.625f;

	float w, h;
	fnt->GetStringDimensions(title, w, h);
	rect.Set(_x - w / 2, _y, _x + w / 2, _y + h);
}

puruGUIMenuItem::~puruGUIMenuItem()
{
	fnt = nullptr;
	snd = nullptr;
}

bool puruGUIMenuItem::Initialize(int _id, puruFont * _fnt, puruSound * _snd, float _x, float _y, char * _title)
{
	id = _id;
	bFocused = false;

	fnt = _fnt;
	snd = _snd;
	title = _title;

	color.red = 1.0f;
	color.green = 1.0f;
	color.blue = 1.0f;

	shadow.red = 0.625f;
	shadow.green = 0.625f;
	shadow.blue = 0.625f;

	float w, h;
	fnt->GetStringDimensions(title, w, h);
	rect.Set(_x - w / 2, _y, _x + w / 2, _y + h);

	return true;
}

void puruGUIMenuItem::Render() {
	if (bFocused)
	{
		fnt->printf(rect.x1 + offset + 3, rect.y1 + 3, title, shadow.red, shadow.green, shadow.blue);
		fnt->printf(rect.x1 - offset, rect.y1 - offset, title, color.red, color.green, color.blue);
	}
	else
	{
		fnt->printf(rect.x1 - offset, rect.y1 - offset, title, shadow.red, shadow.green, shadow.blue);
	}

}

void puruGUIMenuItem::Update(float dt) {

}

void puruGUIMenuItem::Enter() {
	bFocused = true;
	snd->Play();
}

void puruGUIMenuItem::Leave() {
	bFocused = false;
}

bool puruGUIMenuItem::TestPoint(float x, float y)
{
	return rect.TestPoint(x, y);
}
