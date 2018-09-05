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

    class Item {
        friend class Timer;
        Item(unsigned long d, bool r, Callback * cb);
        ~Item() { delete callback; }
        /** millis between calls */
        unsigned long _delay;
        /** last time callback was called */
        unsigned long _called;
        /**
         * @brief if true will be called again
         * Discriminator between callbacks set with setTimeout and setInterval
         */
        bool repeat;
        /** Callback object containing the function */
        Callback * callback;
    };

    /**
     * @brief Callbacks vector
     */
    std::vector<Item*> items;
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
    void setInterval(R (*func)(void), unsigned long timeout) {
        Item * item = new Item(timeout, false, new FuncCallback<R>(func));
        items.push_back(item);
    }
    /**
     * @brief Register a class member function that will be repeatedly called
     *
     * @tparam T Clas object
     * @tparam R method return type
     * @param clbk class member to call
     * @param obj class instance object
     * @param timeout milliseconds of delay between calls
     */
    template <typename T, typename R>
    void setInterval(R (T::*clbk)(), T*obj, unsigned long timeout) {
        Item * item = new Item(timeout, false, new ClsCallback<T,R>(clbk, obj));
        items.push_back(item);
    }
    /**
     * @brief Register a function that will be called once
     *
     * @tparam R function return tipe
     * @param func Function to register
     * @param timeout milliseconds of delay between calls
     */
    template <typename R>
    void setTimeout(R (*func)(void), unsigned long timeout) {
        Item * item = new Item(timeout, true, new FuncCallback<R>(func));
        items.push_back(item);
    }
    /**
     * @brief Register a class member function that will be called once
     *
     * @tparam T Clas object
     * @tparam R method return type
     * @param clbk class member to call
     * @param obj class instance object
     * @param timeout milliseconds of delay between calls
     */
    template <typename T, typename R>
    void setTimeout(R (T::*clbk)(), T*obj, unsigned long timeout) {
        Item * item = new Item(timeout, true, new ClsCallback<T,R>(clbk, obj));
        items.push_back(item);
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