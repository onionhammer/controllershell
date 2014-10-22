// TODO:
// - Add *Nix support in Rom class
#include <iostream>

#include "Shell.h"

int main(int argc, char *argv[]) {

    // Allow application to receive joystick events without focus
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    // Initialize SDL, TTF, & Audio Libraries
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) ||
        TTF_Init() ||
        Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
        std::cerr << SDL_GetError();
        return 1;
    }

    // Create a Shell
    Shell shell;

    //Run Shell
    shell.Run();

    SDL_Quit();
    return 0;
}