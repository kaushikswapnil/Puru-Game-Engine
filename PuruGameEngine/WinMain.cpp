#include "PuruGameSystem.h"
#include "puruQuad.h"
#include "puruTexture.h"
#include "puruSprite.h"
#include "puruGUI.h"
#include "puruGUIMenuItem.h"
#include "puruGUIText.h"
#include "puruSound.h"
#include "puruFont.h"

#include<iomanip>


PGE* pge;
float dt = 0.0f;
int FPS = 0;
puruTexture ptex, pmouse;
puruSprite sprMouse;
puruGUIMenuItem *menu1, *menu2;
puruGUIText* text1, *text2;
puruGUI gui;
puruSound snd;
puruFont fnt;
puruQuad pQuad;
int lastid;

int mouseX, mouseY;

void UpdateBG(float dt)
{
	float tx, ty;
	static float t = 0.0f;

	t += dt;
	tx = 20 * cosf(t / 60);
	ty = 20 * sinf(t / 60);

	pQuad.SetTextureLocations(tx, ty);
}

bool FrameFunc() {
	if (pge->Input_IsKeyDown(PGEK_ESCAPE))
	{
		return false;
	}
	UpdateBG(pge->Timer_GetDelta());

	return true;
}

bool RenderFunc() {
	pge->Gfx_BeginScene(0);
	pQuad.RenderQuad();
	gui.Render();
	pge->Gfx_EndScene();
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bool result;

	//Create system object
	pge = PuruGameSystem::GetInstance();

	if(!pge)
	{
		return 0;
	}

	pge->System_SetState(PURU_FRAME, FrameFunc);
	pge->System_SetState(PURU_RENDER, RenderFunc);
	pge->System_SetState(PURU_FPS, 60);

	//Initialize and run system object

	result = pge->Initialize();
	if (result)
	{
		fnt.Initialize("font1data.txt", "font1.dds");
		snd.Initialize("sound01.wav");
		menu1 = new puruGUIMenuItem();
		menu2 = new puruGUIMenuItem();
		text1 = new puruGUIText(3, 300, 300, "Is it working", &fnt);
		text2 = new puruGUIText(3, 300, 400, "I guess so", &fnt);
		menu1->Initialize(1, &fnt, &snd, 300, 100, "Hello Pawan");
		menu2->Initialize(2, &fnt, &snd, 300, 200, "Fuck Pawan");
		gui.AddObject(menu1);
		gui.AddObject(menu2);
		gui.AddObject(text1);
		gui.AddObject(text2);
		pmouse.Initialize(L"cursor.dds", 15, 21);
		sprMouse.Initialize(0, 0, 15, 21, &pmouse);
		gui.Initialize(pge);
		gui.SetFocus(1);
		gui.SetMouse(&sprMouse);
		ptex.Initialize(L"gamebg.dds", 402, 402);
		pQuad.Initialize(&ptex, 0.0f, 0.0f, 800, 600);
		pge->Run();
	}

	//Shutdown and release system object
	
	fnt.Shutdown();
	snd.Shutdown();
	pge->Shutdown();
	pge = nullptr;

	return 0;
}