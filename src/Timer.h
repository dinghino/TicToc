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
     * @brief Wrapper for lambda with captures
     * 
     * This is a workaround to be used when we want to register lambdas
     * with capture until I figure out how to properly define this stuff
     * 
     * @tparam F function
     * @param f lambda
     * @return auto lambda that will be called
     */
    template <class F>
    static auto lambda(F f) {
        static F fn = f;
        return [] { fn(); };
    }

    /**
     * @brief Register a function that will be repeatedly called
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    template <typename R>
    int every(unsigned long timeout, FUNC_CB<R> func) {
    // Timer& every(unsigned long timeout, FUNC_CB<R> func) {
        Callback * callback = Callback::create(func, timeout, true);
        items.push_back(callback);
        return callback->id();
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
    int every(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
    // Timer& every(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
        Callback * callback = Callback::create(clbk, obj, timeout, true);
        items.push_back(callback);
        return callback->id();
    }
    /**
     * @brief Register a function that will be called once
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    template <typename R>
    int once(unsigned long timeout, FUNC_CB<R> func) {
    // Timer& once(unsigned long timeout, FUNC_CB<R> func) {
        Callback * callback = Callback::create(func, timeout, false);
        items.push_back(callback);
        return callback->id();
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
    int once(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
    // Timer& once(unsigned long timeout, MEMBER_CB<R,C> clbk, C*obj) {
        Callback * callback = Callback::create(clbk, obj, timeout, false);
        items.push_back(callback);
        return callback->id();
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