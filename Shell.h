#pragma once

#include "Menu.h"
#include "Input.h"
#include "Settings.h"

class Shell {

public:
    Shell();

	void Run();

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	Input _input;
	Menu _menu;
    bool _gameMode { false };
	bool _keepRunning { true };

	void CreateMenu();

};