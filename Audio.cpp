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
        audio_device      = SDL_OpenAudioDevice(nullptr, 0, &wav_spec, nullptr, 0);

        if (audio_device)
            initialized = true;
        else
            std::cerr << SDL_GetError() << endl;
    }
    else
        std::cerr << SDL_GetError() << endl;
}

AudioEffect::~AudioEffect() {
    if (initialized) {
        SDL_CloseAudioDevice(audio_device);
        SDL_FreeWAV(wav_buffer);
        initialized = false;
    }
}

void AudioEffect::Play() {
    if (initialized == false)
        return;

    // TODO - SDL play sound
    cout << "Playing " << filename << endl;

    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length
    SDL_PauseAudioDevice(audio_device, 0);
}

void AudioEffect::AudioCallback(void *userdata, Uint8 *stream, int len) {
    auto effect = (AudioEffect*)userdata;

    if (effect->audio_len == 0)
        return;

    len = len > effect->audio_len ? effect->audio_len : len;
    SDL_MixAudio(stream, effect->audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    effect->audio_pos += len;
    effect->audio_len -= len;
}

#pragma endregion

#pragma region Audio

Audio::Audio() {
    _menuEffect     = make_shared<AudioEffect>(MENU_SOUND);
    _openEffect     = make_shared<AudioEffect>(OPEN_SOUND);
    _closeEffect    = make_shared<AudioEffect>(CLOSE_SOUND);
    _shutdownEffect = make_shared<AudioEffect>(SHUTDOWN_SOUND);
}

Audio::~Audio() {
    SDL_CloseAudio();
}

void Audio::Play(AudioType key) {
    // Play matching audio
    switch (key) {
        case MenuAudio:     _menuEffect->Play(); break;
        case OpenAudio:     _openEffect->Play(); break;
        case CloseAudio:    _closeEffect->Play(); break;
        case ShutdownAudio: _shutdownEffect->Play(); break;
    }
}

#pragma endregion