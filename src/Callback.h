#ifndef __DNG__CALLBACK_H_
#define __DNG__CALLBACK_H_

#include <functional>

class Callback;
template <typename R> class FuncCallback;
template <typename C, typename R> class ClsCallback;

// Callback wrapper abstract class
class Callback
{
    int _id;
    static int CREATED;

public:
    Callback(unsigned long d, bool r);
    virtual ~Callback() {}

    void operator()();

    virtual Callback* clone() const = 0;
    virtual void call() const = 0;
    virtual bool exists() const = 0;

    inline const int           id()       const { return _id; }
    inline const bool          repeat()   const { return bRepeat; }
    inline const bool          called()   const { return bCalled; }
    inline const unsigned long getDelay() const { return ulDelay; }
    inline const unsigned long lastCall() const { return ulLastCall; }

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
        object = obj;
        method = clbk;
        callback = std::bind(clbk, obj);
    }
    ~ClsCallback() {
        // if (object)   delete &object;
        // if (method)   delete &method;
        if (callback)
            delete &callback;
    }

    void call() const {
        callback();
    }
    bool exists() const {
        return this->callback != nullptr;
    }

    ClsCallback<C,R>* clone() const {
        return new ClsCallback<C,R>(method, object, ulDelay, bRepeat);
    }


private:
    Callable callback;
    Obj object;
    Method method;
};

// wrapper class for function callbacks
template <typename R>
class FuncCallback : public Callback
{
    using Func = std::function<R()>;
public:
    FuncCallback(Func clbk, unsigned long d, bool r) : Callback(d,r), callback(clbk) {}
    ~FuncCallback() {
        if (callback) delete &callback;
    }

    void call() const {
        callback();
    }

    bool exists() const {
        return this->callback != nullptr;
    }

    FuncCallback<R>* clone() const {
        return new FuncCallback<R>(callback, ulDelay, bRepeat);
    }
private:

    Func callback;
};

// #include "PCallback.h"

#endif