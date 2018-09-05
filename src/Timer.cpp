#include "Timer.h"

Timer::Item::Item(unsigned long d, bool r, Callback * cb)
    : _delay(d), _called(millis()), repeat(r), callback(cb) {}

Timer::~Timer()
{
    typename std::vector<Item*>::iterator item = items.begin();
    for (; item != items.end(); ++item) {
        delete *item;
    }
    items.clear();
}

bool Timer::update()
{
    bool didCall = false;
    if (items.empty()) return didCall;
    typename std::vector<Item*>::iterator it = items.begin();
    for (; it != items.end(); ++it)
    {
        Item * item = *it;
        if (millis() - item->_delay > item->_called) {
            (*item->callback)();
            item->_called = millis();
            didCall = true;
            // created with setTimeout, so we don't need it anymore
            if (!item->repeat) delete item;
        }
    }
    return didCall;
}

