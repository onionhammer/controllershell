#include <iostream>
#include "Audio.h"
#include "Settings.h"

using namespace std;

#pragma region Fields

const string MENU_SOUND     = config.get("Audio", "MenuSound", "");
const string OPEN_SOUND     = config.get("Audio", "OpenSound", "");
const string CLOSE_SOUND    = config.get("Audio", "CloseSound", "");
const string SHUTDOWN_SOUND = config.get("Audio", "ShutdownSound", "");

#pragma endregion

#pragma region Audio

Audio::Audio() {

}

Audio::~Audio() {

}

void Audio::Play(AudioType key) {
    // Play matching audio
    string sound;

    switch (key) {
        case MenuAudio:     sound = MENU_SOUND; break;
        case OpenAudio:     sound = OPEN_SOUND; break;
        case CloseAudio:    sound = CLOSE_SOUND; break;
        case ShutdownAudio: sound = SHUTDOWN_SOUND; break;
        default:            sound = ""; break;
    }

    if (sound != "") {
        // TODO - SDL play sound
        cout << "Playing " << sound << endl;
    }
}

#pragma endregion