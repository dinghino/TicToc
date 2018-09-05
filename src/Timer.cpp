#include "Timer.h"

Timer::Timer(unsigned long _delay) : _delay(_delay) {}

Timer::~Timer()
{
    typename std::vector<Callback*>::iterator cb = _callbacks.begin();
    for (; cb != _callbacks.end(); ++cb) {
        delete *cb;
    }
}

bool Timer::update()
 {
    if (_callbacks.empty()) return false;

    if (millis() - called > _delay) {
        typename std::vector<Callback*>::iterator cb = _callbacks.begin();

        for (; cb != _callbacks.end(); ++cb) {
            (**cb)();
        }

        called = millis();
        return true;
    }
    return false;
}

