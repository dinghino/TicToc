#include "TicToc.h"

unsigned int Timer::CREATED = 0;

Timer::Timer(std::function<void()> clbk, unsigned long d, bool r)
    : callback(clbk), bRepeat(r), ulDelay(d), ulLastCall(millis()), _id(++Timer::CREATED)
{
    // this->_id = ++Timer::CREATED;
}

void Timer::call()
{
    callback();
}

void Timer::operator()()
{
    this->ulLastCall = millis();
    this->bCalled = true;
    call();
}

// Chainable -----------------------------------------------------------------

ExtTimer::ExtTimer(std::function<void()> clbk, unsigned long d, bool r)
    : Timer(clbk, d, r) {}

void ExtTimer::call()
{
    // if f_Until returns false means we reached the condition we decided to stop
    // calling the callback
    if (f_Until != nullptr && f_Until())
    {
        this->bRepeat = false;
        return;
    }
    // if f_OnlyIf returns false means we don't want the callback to run
    if (f_OnlyIf != nullptr && !f_OnlyIf())
    {
        return;
    }

    Timer::call();
}
ExtTimer & ExtTimer::until(std::function<bool()> func)
{
    f_Until = func;
    return *this;
}
ExtTimer &ExtTimer::onlyIf(std::function<bool()> func)
{
    f_OnlyIf = func;
    return *this;
}

/**
 * @brief Override checks and timer and call the function
 * Can be useful when chaining commands and we need the function to run instantly
 * after registration and after the defined delay
 */
ExtTimer &ExtTimer::run()
{
    Timer::call();
    return *this;
}