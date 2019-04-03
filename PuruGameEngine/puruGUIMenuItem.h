#pragma once

#include "puruGUIObject.h"
#include "puruRect.h"

class puruFont;
class puruSound;

class puruGUIMenuItem : public puruGUIObject {
public:
	puruGUIMenuItem();
	puruGUIMenuItem(int id, puruFont *fnt, puruSound* snd, float x, float y, char *title);
	virtual ~puruGUIMenuItem() override;

	bool Initialize(int id, puruFont *fnt, puruSound* snd, float x, float y, char *title);

	virtual void Render() override;
	virtual void Update(float dt) override;

	virtual void Enter() override;
	virtual void Leave() override;

	virtual bool TestPoint(float x, float y) override;

private:
	puruFont *fnt;
	puruSound* snd;
	char *title;

	guiObjectColor shadow, color;
	float offset;
};
