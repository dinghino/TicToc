#include "Timer.h"

int Callback::CREATED = 0;

Callback::Callback(unsigned long d, bool r)
    : bRepeat(r), ulDelay(d), ulLastCall(millis())
{
    this->_id = ++Callback::CREATED;
}

void Callback::operator()()
{
    call();
    this->ulLastCall = millis();
    this->bCalled = true;
}
