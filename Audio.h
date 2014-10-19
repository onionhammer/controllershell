#pragma once

#include <memory>
#include <SDL2/SDL_mixer.h>

enum AudioType {
    MENU_AUDIO = 0,
    OPEN_AUDIO,
    CLOSE_AUDIO,
    SHUTDOWN_AUDIO
};

class AudioEffect {
    friend class Audio;

public:
    AudioEffect(std::string filename);
    ~AudioEffect();
    void Play();

private:
    Mix_Chunk* _audio;
    std::string _filename;
    bool _initialized { false };
};

class Audio {

public:
    Audio();
    ~Audio();
    void Play(AudioType key);
    bool hasShutdown() const { return _shutdownEffect._initialized; }

private:
    AudioEffect _menuEffect;
    AudioEffect _openEffect;
    AudioEffect _closeEffect;
    AudioEffect _shutdownEffect;
};