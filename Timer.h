#pragma once

#include <SDL2/SDL.h>

class Timer {

public:
    Timer(int interval, SDL_TimerCallback callback);
    ~Timer();

    void Start();
    void Stop();
    void Reset();

private:
    int _interval;
    SDL_TimerID _timer{ 0 };
    SDL_TimerCallback _callback;
};