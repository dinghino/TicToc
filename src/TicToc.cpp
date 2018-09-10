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

    typename std::vector<Timer*>::iterator it = items.begin();
    for (; it != items.end();)
    {
        Timer * cb = *it;

        // delete the callback cb if needed
        if (!cb->exists() || (cb->called() && !cb->repeat())) {
            it = items.erase(it); // also delete cb?
            continue;
        } else if (millis() - cb->lastCall() >= cb->getDelay()) {
            didCall = true;
            (*cb)();
        }
        ++it;
    }
    return didCall;
}

