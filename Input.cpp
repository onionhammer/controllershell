#include "Input.h"
#include "Settings.h"

using namespace std;

#pragma region Fields

const int TAPS_TO_QUIT    = config.get("Behavior", "TapsToQuit", 3);
const int QUIT_TIMEOUT    = config.get("Behavior", "TapsToQuitTimeout", 400); //Ms
const int SCROLL_INTERVAL = config.get("Behavior", "ScrollInterval", 150); //Ms
const int SCROLL_START    = config.get("Behavior", "ScrollStart", 200); //Ms

#pragma endregion

#pragma region Timer Events

Uint32 push_event(Uint32 interval, IntervalEvents type) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.code = type;
    userevent.type = SDL_USEREVENT;
    event.user     = userevent;

    SDL_PushEvent(&event);
    return interval;
}

#define DEF_TIMER(name, body) \
    Uint32 name(Uint32 interval, void *param) { return body; }

    DEF_TIMER(scrollIntervalTimer, push_event(interval, INT_SCROLLINTERVAL))
    DEF_TIMER(scrollTimer,         push_event(interval, INT_SCROLL) * 0)
    DEF_TIMER(quitTimer,           push_event(interval, INT_QUIT) * 0)
#undef DEF_TIMER

#pragma endregion

#pragma region Input

// Constructor(s)
Input::Input() :
    _scrollIntervalTimer(SCROLL_INTERVAL, scrollIntervalTimer),
    _scrollTimer(SCROLL_START, scrollTimer),
    _quitTimer(QUIT_TIMEOUT, quitTimer) {

	int numJoysticks = SDL_NumJoysticks();

	for (int i = 0; i < numJoysticks; ++i)
		_joysticks.push_back(SDL_JoystickOpen(i));

	if (_joysticks.size())
		cout << "All joysticks bound." << endl;
    else
		cerr << "No Joysticks found." << endl;
}

Input::~Input() {
	for (auto joystick : _joysticks)
		if (SDL_JoystickGetAttached(joystick))
			SDL_JoystickClose(joystick);
}

// Methods
void Input::InitMappings() {
    int i(0);

    auto mapping = config.get("Settings", "JoyMapping", "Default");
    for (auto value = JoyButtonNames.begin(); value < JoyButtonNames.end(); ++value)
        _buttonMap[config.get(mapping, *value, 0)] = JoyButtons(i++);
}

void Input::HandleEvent(SDL_Event e, Menu& menu) {
    switch (e.type) {
        case SDL_JOYBUTTONUP:
            // Stop timer
            switch (_buttonMap[e.jbutton.button]) {
                case JOY_UP:
                case JOY_DOWN:
                    // Stop timer(s)
                    _scrollTimer.Stop();
                    _scrollIntervalTimer.Stop();
                    cout << "Scroll stopped." << endl;
                    break;
            }
            break;

		case SDL_JOYBUTTONDOWN:
            // Handle joystick button
            cout << "btn: " << (int)e.jbutton.button << endl;

            switch (_buttonMap[e.jbutton.button]) {
                case JOY_UP:
                    menu.TriggerNavigate(NAV_UP);
					_lastNav = NAV_UP;
					_scrollTimer.Reset(); // Start scroll timer
                    break;

                case JOY_DOWN:
                    menu.TriggerNavigate(NAV_DOWN);
					_lastNav = NAV_DOWN;
					_scrollTimer.Reset(); // Start scroll timer
                    break;

                case JOY_LEFT:  menu.TriggerNavigate(NAV_BACK); break;
                case JOY_RIGHT: menu.TriggerClick(false); break;
                case JOY_START:
                case JOY_A:     menu.TriggerClick(true); break;
            }
            break;

		case SDL_KEYDOWN:
            // Handle keyboard
			switch (e.key.keysym.sym) {
				case SDLK_UP:	  menu.TriggerNavigate(NAV_UP); break;
				case SDLK_DOWN:   menu.TriggerNavigate(NAV_DOWN); break;
				case SDLK_LEFT:   menu.TriggerNavigate(NAV_BACK); break;
				case SDLK_RIGHT:  menu.TriggerClick(false); break;
				case SDLK_RETURN: menu.TriggerClick(true); break;
			}
			break;
	}
}

void Input::HandleIntervalEvent(SDL_Event e, Menu& menu) {
    switch (e.user.code) {
        case INT_QUIT:
            // Reset exit taps
            _exitTaps = 0;
            break;

        case INT_SCROLL:
            // Start scrolling timer
            _scrollIntervalTimer.Reset();
            break;

        case INT_SCROLLINTERVAL:
            // Re-trigger last NavUP/DOWN event
			menu.TriggerNavigate(_lastNav);
            break;
    }
}

bool Input::HandleGameMode(SDL_Event e) {
    if (e.type == SDL_JOYBUTTONDOWN) {
        switch (_buttonMap[e.jbutton.button]) {
            case JOY_Q:
                //Reset quit timer
                _quitTimer.Reset();

                // Count up number of times this was pressed
				if (++_exitTaps >= TAPS_TO_QUIT) {
                    _exitTaps = 0;
                    return true;
                }
                return false;

            default:
				// Clear counter
				_exitTaps = 0;
				break;
        }
    }

    return false;
}

#pragma endregion