#ifndef __DNG__TIMER_H__
#define __DNG__TIMER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

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
     * @brief Callbacks vector
     */
    std::vector<Callback*> items;

    /**
     * Type definition for clarity of code.
     * R return type, C type of object
     */
    template <typename R>
    using FUNC_CB = R(*)(void);
    template <typename R, typename C>
    using MEMBER_CB = R(C::*)(void);

public:
    ~Timer();
    /**
     * @brief Register a function that will be repeatedly called
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    template <typename R>
    Callback& every(unsigned long timeout, FUNC_CB<R> func) {
        Callback * callback = Callback::create(func, timeout, true);
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
    template <typename C, typename R>
    Callback& every(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
        Callback * callback = Callback::create(clbk, obj, timeout, true);
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
    template <typename R>
    Callback& once(unsigned long timeout, FUNC_CB<R> func) {
        Callback * callback = Callback::create(func, timeout, false);
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
    template <typename C, typename R>
    Callback& once(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
        Callback * callback = Callback::create(clbk, obj, timeout, false);
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