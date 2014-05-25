// TODO:
// - Add *Nix support in Rom class
#include <iostream>

#include "Shell.h"

int main(int argc, char *argv[]) {

    // Allow application to receive joystick events without focus
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    // Initialize SDL & TTF Libraries
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) std::cerr << SDL_GetError();
    if (TTF_Init()) std::cerr << SDL_GetError();

    // Create a Shell
    Shell shell;

    //Run Shell
    shell.Run();

    SDL_Quit();
    return 0;
}
