#ifndef __DNG__TIMER_H__
#define __DNG__TIMER_H__

#include <Arduino.h>
#include <vector>
#include <string>

#include "Callback.h"

/**
 * @brief Timer object for Arduino projects
 * Can be used to desync calls to functions and class members with no parameters
 * To be called every x ms, defined upon object creation.
 */
class Timer
{
private:
    /**
     * Time keeping variables
     */
    unsigned long called, _delay;
    /**
     * @brief Vector of callbacks
     */
    std::vector<Callback*> _callbacks;

public:
    Timer(unsigned long _delay);
    ~Timer();

    template <typename R>
    void addCallback(R (*func)(void)) {
        _callbacks.push_back(new FuncCallback<R>(func));
    }
    template <typename T, typename R>
    void addCallback(R (T::*clbk)(), T*obj) {
        _callbacks.push_back(new ClsCallback<T,R>(clbk, obj));
    }
    /**
     * @brief Update the timer and call the callback if it's time
     * 
     * @return true callback was called
     * @return false callbakc NOT called
     */
    bool update();
};


#endif