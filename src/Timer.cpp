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

        // delete the callback cb if needed
        if (!cb->exists() || (cb->called() && !cb->repeat())) {
            Serial.printf("!!! Deleting callback %d\n", cb->id());
            // FIXME: Item does not get destroyes in aObject case
            // So even if the source object does not exists... cb gets called.. duh?
            // delete cb;
            it = items.erase(it);
            continue;
        } else if (millis() - cb->lastCall() > cb->getDelay()) {
            didCall = true;
            (*cb)();
        }
        ++it;
    }
    return didCall;
}

