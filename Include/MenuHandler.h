#pragma once

class puruGUI;
class puruFont;
class puruSound;
class puruSprite;
//My Class to handle menu changes
class MenuHandler {

public:
	MenuHandler();
	~MenuHandler();

	void LoadMainMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr);
	void LoadCreditsMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr);
	void LoadInstructionsMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr);
	void LoadGameOverMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr);
	void LoadGameWonMenu(puruGUI *gui, puruFont *fnt, puruSound* snd, puruSprite* cursorspr);
	void DeleteMainMenu(puruGUI *gui);
	void DeleteCreditsMenu(puruGUI *gui);
	void DeleteInstructionsMenu(puruGUI *gui);
	void DeleteGameOverMenu(puruGUI *gui);
	void DeleteGameWonMenu(puruGUI *gui);
	void loadGUIOptions(puruGUI *gui, puruSprite*cursorspr);
};