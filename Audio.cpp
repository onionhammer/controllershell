#include <iostream>

#include "Audio.h"
#include "Settings.h"

using namespace std;

#pragma region Fields

const string MENU_SOUND     = config.get("Audio", "MenuSound", "");
const string OPEN_SOUND     = config.get("Audio", "OpenSound", "");
const string CLOSE_SOUND    = config.get("Audio", "CloseSound", "");
const string SHUTDOWN_SOUND = config.get("Audio", "ShutdownSound", "");
const int VOLUME            = config.get("Audio", "Volume", 5);

#pragma endregion

#pragma region AudioEffect

AudioEffect::AudioEffect(string filename) : _filename(filename) {
    if (_filename == "")
        return;

    // Load audio file
    _audio = Mix_LoadWAV(filename.c_str());
    _audio->volume = VOLUME;

    if (_audio)
        _initialized = true;
    else
        cerr << SDL_GetError() << endl;
}

AudioEffect::~AudioEffect() {
    if (_initialized) {
        Mix_FreeChunk(_audio);
        _initialized = false;
    }
}

void AudioEffect::Play() {
    if (_initialized) {
        // SDL Mixer - play sound
        cout << "Playing " << _filename << endl;

        if (Mix_PlayChannel(-1, _audio, 0) == -1)
            cerr << SDL_GetError() << endl;
    }
}

#pragma endregion

#pragma region Audio

Audio::Audio() :
    _menuEffect(MENU_SOUND),
    _openEffect(OPEN_SOUND),
    _closeEffect(CLOSE_SOUND),
    _shutdownEffect(SHUTDOWN_SOUND) {
}

Audio::~Audio() {
    Mix_CloseAudio();
}

void Audio::Play(AudioType key) {
    // Play matching audio
    switch (key) {
        case MenuAudio:     _menuEffect.Play(); break;
        case OpenAudio:     _openEffect.Play(); break;
        case CloseAudio:    _closeEffect.Play(); break;
        case ShutdownAudio: _shutdownEffect.Play(); break;
    }
}

#pragma endregion