#include "TicToc.h"

TicToc::~TicToc()
{
    typename std::vector<Timer*>::iterator cb = items.begin();
    for (; cb != items.end(); ++cb) {
        delete *cb;
    }
    items.clear();
}

bool TicToc::update()
{
    bool didCall = false;
    if (items.empty()) return didCall;

    typename std::vector<Timer*>::iterator timerIt = items.begin();
    for (; timerIt != items.end();)
    {
        Timer * timer = *timerIt;
        // delete the callback timer if needed
        if (!timer->exists() || (timer->called() && !timer->repeat())) {
            timerIt = items.erase(it); // also delete timer?
            continue;
        } else if (millis() - timer->lastCall() >= timer->getDelay()) {
            didCall = true;
            (*timer)();
        }
        ++timerIt;
    }
    return didCall;
}

bool TicToc::clear(Timer &timer)
{
    return clear(timer.id());
}

bool TicToc::clear(Timer *timerPtr)
{
    return clear(timerPtr->id());
}

bool TicToc::clear(unsigned int id)
{
    typename std::vector<Timer *>::iterator current = items.begin();
    for (; current != items.end(); current++)
    {
        if ((**current).id() == id)
            (**current).repeat(false);
            return true;
    }
    // no timer with given id found
    return false;
}