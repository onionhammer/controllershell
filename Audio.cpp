#include <iostream>
#include "SDL.h"
#include "SDL_audio.h"

#include "Audio.h"
#include "Settings.h"

using namespace std;

#pragma region Fields

const string MENU_SOUND     = config.get("Audio", "MenuSound", "");
const string OPEN_SOUND     = config.get("Audio", "OpenSound", "");
const string CLOSE_SOUND    = config.get("Audio", "CloseSound", "");
const string SHUTDOWN_SOUND = config.get("Audio", "ShutdownSound", "");

#pragma endregion

#pragma region AudioEffect

AudioEffect::AudioEffect(string filename) :
    filename(filename) {

    if (filename == "")
        return;

    // Load audio file
    if (SDL_LoadWAV(filename.c_str(), &wav_spec, &wav_buffer, &wav_length)) {
        // Set up callback
        wav_spec.callback = AudioEffect::AudioCallback;
        wav_spec.userdata = this;
        initialized       = true;
    }
    else
        std::cerr << SDL_GetError() << endl;
}

AudioEffect::~AudioEffect() {
    if (initialized) {
        SDL_FreeWAV(wav_buffer);
        initialized = false;
    }
}

void AudioEffect::Play() {
    if (initialized == false)
        return;

    // TODO - SDL play sound
    cout << "Playing " << filename << endl;
    if (SDL_OpenAudio(&wav_spec, nullptr) < 0)
        cout << "Failed to open audio file " << filename << endl;

    SDL_PauseAudio(0);

}

void AudioEffect::AudioCallback(void *userdata, Uint8 *stream, int len) {
    auto effect = static_cast<AudioEffect*>(userdata);

}

#pragma endregion

#pragma region Audio

Audio::Audio():
    _menuEffect(MENU_SOUND),
    _openEffect(OPEN_SOUND),
    _closeEffect(CLOSE_SOUND),
    _shutdownEffect(SHUTDOWN_SOUND) {
    
}

Audio::~Audio() {
    SDL_CloseAudio();
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