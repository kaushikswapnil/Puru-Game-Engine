#include"MenuHandler.h"
#include "puruGUI.h"
#include "puruGUIMenuItem.h"
#include "puruGUIText.h"
#include "puruFont.h"
#include "puruSound.h"
#include "puruSprite.h"

MenuHandler::MenuHandler() { }
MenuHandler::~MenuHandler() { }

void MenuHandler::LoadMainMenu(puruGUI* gui, puruFont* fnt, puruSound* snd, puruSprite* cursorspr) {
	gui->AddObject(new puruGUIMenuItem(1, fnt, snd, 400, 220, "Play"));
	gui->AddObject(new puruGUIMenuItem(2, fnt, snd, 400, 260, "Instructions"));
	gui->AddObject(new puruGUIMenuItem(3, fnt, snd, 400, 300, "Credits"));
	gui->AddObject(new puruGUIMenuItem(4, fnt, snd, 400, 340, "Exit"));
	loadGUIOptions(gui, cursorspr);
}

void MenuHandler::LoadInstructionsMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr) {
	std::string instructions = "Kill all the aliens. Avoid their bullets.";
	gui->AddObject(new puruGUIText(1, 400, 300, instructions.c_str(), fnt));
	gui->AddObject(new puruGUIMenuItem(2, fnt, snd, 400, 400, "Back to Main Menu"));
	loadGUIOptions(gui, cursorspr);
}

void MenuHandler::LoadCreditsMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr) {
	std::string credits = "Made by Puru Kaushik.";
	gui->AddObject(new puruGUIText(1, 400, 300, credits.c_str(), fnt));
	gui->AddObject(new puruGUIMenuItem(2, fnt, snd, 400, 360, "Back to Main Menu"));
	loadGUIOptions(gui, cursorspr);
}

void MenuHandler::LoadGameOverMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr) {
	std::string endgame = "Game Over. Press space to go back to Main Menu";
	gui->AddObject(new puruGUIText(1, 400, 300, endgame.c_str(), fnt));
	loadGUIOptions(gui, cursorspr);
}

void MenuHandler::LoadGameWonMenu(puruGUI * gui, puruFont * fnt, puruSound* snd, puruSprite * cursorspr)
{
	std::string wongame = "Congratulations!";
	std::string wongame2 = "You have won the game.";
	std::string wongame3 = "Press space to go back to Main Menu";
	puruGUIText* hgeText = new puruGUIText(1, 400, 200, wongame.c_str(), fnt);
	gui->AddObject(hgeText);
	puruGUIText* hgeText2 = new puruGUIText(2, 400, 300, wongame2.c_str(), fnt);
	gui->AddObject(hgeText2);
	puruGUIText* hgeText3 = new puruGUIText(3, 400, 400, wongame3.c_str(), fnt);
	gui->AddObject(hgeText3);
	loadGUIOptions(gui, cursorspr);
}

void MenuHandler::DeleteMainMenu(puruGUI *gui) {
	for (int i = 1; i <= 4; i++)
		gui->DelObject(i);
}

void MenuHandler::DeleteInstructionsMenu(puruGUI *gui) {
	for (int i = 1; i <= 3; i++)
		gui->DelObject(i);
}

void MenuHandler::DeleteCreditsMenu(puruGUI *gui) {
	for (int i = 1; i <= 2; i++)
		gui->DelObject(i);
}

void MenuHandler::DeleteGameOverMenu(puruGUI *gui) {
	for (int i = 1; i <= 1; i++)
		gui->DelObject(i);
}

void MenuHandler::DeleteGameWonMenu(puruGUI * gui)
{
	for (int i = 1; i <= 3; i++)
		gui->DelObject(i);
}

void MenuHandler::loadGUIOptions(puruGUI *gui, puruSprite* cursorspr) {
	gui->SetMouse(cursorspr);
	gui->SetFocus(1);
	gui->Enter();
}