#pragma once

#include <memory>

enum AudioType {
    MenuAudio = 0,
    OpenAudio,
    CloseAudio,
    ShutdownAudio
};

class AudioEffect {
public:
    AudioEffect(std::string filename);
    ~AudioEffect();
    void Play();

private:
    static void AudioCallback(void *userdata, Uint8 *stream, int len);

    Uint32 wav_length;
    Uint8* wav_buffer;

    Uint32 audio_len;
    Uint8* audio_pos;
    SDL_AudioDeviceID audio_device;

    SDL_AudioSpec wav_spec;
    std::string filename;
    bool initialized { false };
};

class Audio {

public:
    Audio();
    ~Audio();
    void Play(AudioType key);

private:
    std::shared_ptr<AudioEffect> _menuEffect;
    std::shared_ptr<AudioEffect> _openEffect;
    std::shared_ptr<AudioEffect> _closeEffect;
    std::shared_ptr<AudioEffect> _shutdownEffect;
};