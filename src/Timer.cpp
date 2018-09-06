#include "Timer.h"

Timer::~Timer()
{
    typename std::vector<Callback*>::iterator cb = items.begin();
    for (; cb != items.end(); ++cb) {
        delete *cb;
    }
    items.clear();
}

bool Timer::update()
{
    bool didCall = false;
    if (items.empty()) return didCall;

    typename std::vector<Callback*>::iterator it = items.begin();
    for (; it != items.end();)
    {
        Callback * cb = *it;
        if (millis() - cb->lastCall() > cb->getDelay()) {
            didCall = true;
            (*cb)();
        }
        // delete the callback cb if needed
        if (cb->called() && !cb->repeat()) {
            it = items.erase(it);
            delete cb;
        } else {
            ++it;
        }
    }
    return didCall;
}

