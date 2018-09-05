#include "Timer.h"

Timer::Item::Item(unsigned long d, bool r, Callback * cb)
    : _delay(d), _last_call(millis()), repeat(r), callback(cb) {}

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
    for (; it != items.end();)
    {
        Item * item = *it;
        if (millis() - item->_last_call > item->_delay) {
            didCall = true;
            (*item->callback)();
            item->_last_call = millis();
            item->_called = true;
        }
        // delete the callback item if needed
        if (item->_called && !item->repeat) {
            it = items.erase(it);
        } else {
            ++it;
        }
    }
    return didCall;
}

