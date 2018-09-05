#ifndef __DNG__CALLBACK_H_
#define __DNG__CALLBACK_H_

class Callback;
template <typename R> class FuncCallback;
template <typename C, typename R> class ClsCallback;

// Callback wrapper abstract class
class Callback
{
public:
    virtual ~Callback() {}
    virtual Callback* clone() const = 0;
    void operator()() { call(); }
private:
    virtual void call() const = 0;

    template <typename R, typename C>
    static Callback * create(R (C::*clbk)(), C*obj) {
        return new ClsCallback<C,R>(clbk, obj);
    }
    template <typename R>
    static Callback * create(R (*clbk)())
    {
        return new FuncCallback<R>(clbk);
    }
};

// template for class method callbacks
template <typename C, typename R>
class ClsCallback : public Callback
{
public:
    ClsCallback(R(C::*clbk)(), C*obj) : object(obj), callback(clbk) {}
    Callback* clone() const { return new ClsCallback<C,R>(callback, object); }

private:
    void call() const { (object->*callback)(); }

    C* object;
    R (C::*callback)();
};

// wrapper class for function callbacks
template <typename R>
class FuncCallback : public Callback
{
public:
    FuncCallback(R (*clbk)()) : callback(clbk) {}
    Callback* clone() const { return new FuncCallback<R>(callback); }
private:
    void call() const { (*callback)(); }

    R(*callback)();
};

#include "PCallback.h"

#endif