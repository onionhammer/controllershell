#pragma once

#include <iostream>
#include <map>

#include "Menu.h"
#include "Timer.h"

enum JoyButtons {
	JOY_UP = 0,
	JOY_RIGHT,
	JOY_LEFT,
	JOY_DOWN,
	JOY_START,
	JOY_A,
    JOY_Q
};

const auto JoyButtonNames {
    "JoyUP",
    "JoyRIGHT",
    "JoyLEFT",
    "JoyDOWN",
    "JoySTART",
    "JoyA",
    "JoyQ"
};

enum IntervalEvents {
    INT_QUIT,
    INT_SCROLL,
    INT_SCROLLINTERVAL
};

class Input {
    
public:
	// Constructor
	Input();
	~Input();

    // Methods
	void InitMappings();
    void HandleEvent(SDL_Event e, Menu& menu);
    void HandleIntervalEvent(SDL_Event e, Menu& menu);
    bool HandleGameMode(SDL_Event e);
    
private:
	std::vector<SDL_Joystick*> _joysticks;
	std::map<int, JoyButtons> _buttonMap;
    Timer _quitTimer, 
        _scrollTimer,
        _scrollIntervalTimer;
	MoveDirection _lastNav;
    int _exitTaps { 0 };
};