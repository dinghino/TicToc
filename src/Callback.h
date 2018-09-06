#ifndef __DNG__CALLBACK_H_
#define __DNG__CALLBACK_H_

#include <functional>

class Callback;
template <typename R> class FuncCallback;
template <typename C, typename R> class ClsCallback;

// Callback wrapper abstract class
class Callback
{
public:
    Callback(unsigned long d, bool r)
        : bRepeat(r), ulDelay(d), ulLastCall(millis()) {}

    virtual ~Callback() {}

    virtual Callback* clone() const = 0;

    void operator()() {
        call();
        this->ulLastCall = millis();
        this->bCalled = true;
    }

    const unsigned long getDelay() const { return ulDelay; }
    const unsigned long lastCall() const { return ulLastCall; }
    const bool repeat() const { return bRepeat; }
    const bool called() const { return bCalled; }

    // factory methods
    template <typename R>
    static FuncCallback<R> * create(R(*clbk)(),unsigned long d, bool r) {
        return new FuncCallback<R>(clbk,d,r);
    }
    template <typename C, typename R>
    static ClsCallback<C,R> * create(R(C::*clbk)(), C*obj,unsigned long d, bool r) {
        return new ClsCallback<C,R>(clbk, obj,d,r);
    }

protected:
    friend class Timer;

    bool bCalled;
    bool bRepeat;
    unsigned long ulDelay;
    unsigned long ulLastCall;

    virtual void call() const = 0;

};

// template for class method callbacks
template <typename C, typename R>
class ClsCallback : public Callback
{
    using Method = std::function<R(C*)>;
    using Callable = std::function<R(void)>;
    using Obj = C*;
public:
    ClsCallback(Method clbk, Obj obj,unsigned long d, bool r) : Callback(d,r) {
        callable = std::bind(clbk, obj);
    }
    Callback* clone() const {
        return new ClsCallback<C,R>(callback, object, ulDelay, bRepeat);
    }

private:
    void call() const { callable(); }
    Callable callable;
    Obj object;
    Method callback;
};

// wrapper class for function callbacks
template <typename R>
class FuncCallback : public Callback
{
    using Func = std::function<R()>;
public:
    FuncCallback(Func clbk, unsigned long d, bool r) : Callback(d,r), callback(clbk) {}
    Callback* clone() const {
        return new FuncCallback<R>(callback, ulDelay, bRepeat);
    }
private:
    void call() const { callback(); }

    Func callback;
};

#include "PCallback.h"

#endif