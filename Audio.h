#pragma once

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
    AudioEffect _menuEffect;
    AudioEffect _openEffect;
    AudioEffect _closeEffect;
    AudioEffect _shutdownEffect;
};