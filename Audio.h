#pragma once

enum AudioType {
    MenuAudio = 0,
    OpenAudio,
    CloseAudio,
    ShutdownAudio
};

class Audio {

public:
    Audio();
    ~Audio();
    void Play(AudioType key);

};