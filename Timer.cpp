#include "Timer.h"

Timer::Timer(int interval, SDL_TimerCallback callback) :
    _interval(interval), _callback(callback) {
    _timer = 0;
}

Timer::~Timer() {
    Stop();
}

void Timer::Start() {
    _timer = SDL_AddTimer(_interval, _callback, nullptr);
}

void Timer::Stop() {
    if (_timer) {
        SDL_RemoveTimer(_timer);
        _timer = 0;
    }
}

void Timer::Reset() {
    Stop();
    Start();
}