#ifndef __DNG__TIMER_H__
#define __DNG__TIMER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <functional>
#include <vector>
#include <string>

#include "Timer.h"

/**
 * @brief Timer object for Arduino projects
 * Can be used to desync calls to functions and class members with no parameters
 * To be called every x ms, defined upon object creation.
 */
class TicToc
{
private:

    /**
     * @brief Timer callbacks vector
     */
    std::vector<Timer*> items;

    /**
     * Type definition for clarity of code.
     * R return type, C type of object
     */
    using FUNC_CB = void(*)(void);
    template <typename C>
    using MEMBER_CB = void(C::*)(void);

public:
    ~TicToc();

    /**
     * @brief Wrapper for lambda with captures
     * 
     * This is a workaround to be used when we want to register lambdas
     * with capture until I figure out how to properly define this stuff
     * 
     * @tparam F function
     * @param f lambda
     * @return auto lambda that will be called
     */
    // template <class F>
    // static auto lambda(F f) {
    //     static F fn = f;
    //     return [] { fn(); };
    // }

    /**
     * @brief Register a function that will be repeatedly called
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    ExtTimer & every(unsigned long timeout, FUNC_CB func)
    {
        ExtTimer * callback = new ExtTimer(func, timeout, true);
        items.push_back(callback);
        return *callback;
    }
    /**
     * @brief Register a class member function that will be repeatedly called
     *
     * @tparam C Clas object
     * @tparam R method return type
     * @param clbk class member to call
     * @param obj class instance object
     * @param timeout milliseconds of delay between calls
     */
    template <typename C>
    ExtTimer & every(unsigned long timeout, MEMBER_CB<C> clbk, C*obj)
    {
        ExtTimer * callback = new ExtTimer(clbk, obj, timeout, true);
        items.push_back(callback);
        return *callback;
    }
    /**
     * @brief Register a function that will be called once
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    Timer & once(unsigned long timeout, FUNC_CB func)
    {
        Timer * callback = new Timer(func, timeout, false);
        items.push_back(callback);
        return *callback;
    }
    /**
     * @brief Register a class member function that will be called once
     *
     * @tparam C Clas object
     * @tparam R method return type
     * @param clbk class member to call
     * @param obj class instance object
     * @param timeout milliseconds of delay between calls
     */
    template <typename C>
    Timer & once(unsigned long timeout, MEMBER_CB<C> clbk, C*obj)
    {
        Timer * callback = new Timer(clbk, obj, timeout, false);
        items.push_back(callback);
        return *callback;
    }
    /**
     * @brief Update the timer and call the callback if it's time
     * 
     * @return true at least one callback was called
     * @return false no callbacks were called
     */
    bool update();
};

#endif