#include <tuple>

#include "Shell.h"
#include "Settings.h"
#include "StringUtilities.h"
#include "Rom.h"

using namespace std;

#pragma region Fields

const string TITLE         = config.get("Appearance", "Title", "Controller Shell");
const string BG_COLOR      = config.get("Appearance", "Background", "0, 0, 0");
const string POWER_TEXT    = "power off";
const string POWER_WARNING = "are you sure?";
const string SHUTDOWN_CMD  = config.get("Settings", "Shutdown", "");

const bool USE_FULLSCREEN  = config.getBool("Settings", "Fullscreen", true);
const bool USE_GL          = config.getBool("Settings", "UseOpenGL", false);

const tuple<int, int> DIMENSIONS{
    config.get("Appearance", "Width", 1422),
    config.get("Appearance", "Height", 800)
};

#pragma endregion

#pragma region Shell

Shell::Shell() {
    // Load settings
    int flags{ },
            x{ SDL_WINDOWPOS_UNDEFINED },
            y{ SDL_WINDOWPOS_UNDEFINED };

    int renderflags { SDL_RENDERER_PRESENTVSYNC };

    if (USE_GL) {
        flags       |= SDL_WINDOW_OPENGL;
        renderflags |= SDL_RENDERER_ACCELERATED;
    }
    else
        renderflags |= SDL_RENDERER_SOFTWARE;

    if (USE_FULLSCREEN) {
        flags |= SDL_WINDOW_BORDERLESS
              |  SDL_WINDOW_MAXIMIZED
              |  SDL_WINDOW_INPUT_FOCUS;

        x = 0, y = 0;
    }

    // Open an SDL Window
    _window = SDL_CreateWindow(TITLE.c_str(),
        x, y, get<0>(DIMENSIONS), get<1>(DIMENSIONS),
        flags);
    if (_window == nullptr) cerr << SDL_GetError();

    // Hide the cursor
    SDL_ShowCursor(0);

    // Create SDL Renderer
    _renderer = SDL_CreateRenderer(_window, 0, renderflags);
    if (_renderer == nullptr) cerr << SDL_GetError();

    // Initialize joystick input mappings
    _input.InitMappings();

    // Create menu
    _menu.setScreenHeight(get<1>(DIMENSIONS));
    CreateMenu();
}

// Create a menu system
void Shell::CreateMenu() {
    auto emulators = config.get("Settings", "Emulators", "");

    // Load console with roms
    for (auto emulator : split(emulators, ',')) {
        // Retrieve console info from config &
        // create the top level menu item
        auto console  = config.getConsole(emulator);
        auto topLevel = _menu.Add(console.Name);

        // Iterate through and add files
        for (auto romFile : Rom::Find(console))
            // Run the ROM
            topLevel
                ->Add(Rom::CleanName(romFile))
                ->OnClick([=] {
                    // Run the emulator with the input rom
                    Rom::Run(console, console.RomPath + romFile);

                    // Set "game mode" to true
                    _gameMode = true;
                });
    }

    // Add Power-Off option
    _menu.Add(POWER_TEXT)
        ->Add(POWER_WARNING)
        ->OnClick([=] {
            // Shut down computer
            if (SHUTDOWN_CMD.size())
                system(SHUTDOWN_CMD.c_str());

            _keepRunning = false;
        });
}

// Run the shell loop
void Shell::Run() {
    #define RGB(p) (Uint8)stoi(p[0]), (Uint8)stoi(p[1]), (Uint8)stoi(p[2])
    const auto RGB_BACKGROUND = split(BG_COLOR, ',');
    const SDL_Color BG = { RGB(RGB_BACKGROUND) };

    // Render loop
    SDL_Event event;

    while (_keepRunning) {

        // Process event loop
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
               (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
                break;

            switch (event.type) {
                case SDL_JOYBUTTONUP:
                case SDL_JOYBUTTONDOWN:
                case SDL_KEYDOWN:
                    // Submit event to input
                    if (_gameMode == false)
                        _input.HandleEvent(event, _menu);
                    else if (_input.HandleGameMode(event))
                        Rom::Exit(); // Exit the game
                    break;

                case SDL_JOYAXISMOTION:
                    if (_gameMode)
                        _input.HandleGameMode(event);

                case SDL_WINDOWEVENT:
                    // If app regains focus, switch off game mode
                    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                        _gameMode = false;
                    break;

                case SDL_RENDER_TARGETS_RESET:
                    // Reset all texture flags in menu
                    cout << "Render Targets Reset" << endl;
                    _menu.ResetTextures();
                    break;

                case SDL_USEREVENT:
                    // Pass back to input
                    _input.HandleIntervalEvent(event, _menu);
                    break;
            }
        }

        // Clear the scene
        SDL_SetRenderDrawColor(_renderer, BG.r, BG.g, BG.b, 255);
        SDL_RenderClear(_renderer);

        // Render menu
        _menu.Render(_renderer);

        // Present scene
        SDL_RenderPresent(_renderer);
    }
}

#pragma endregion