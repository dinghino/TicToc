/**
 * @brief Templates for parametrized Callbacks
 *
 * This stuff is work in progress and most probably will be completely deprecated
 * and removed soon. It's not included in any other header nor cpp file but was
 * in the history and for now it stays here.
 */

#ifndef __DNG__PARAMETRIZED_CALLBACK_H__
#define __DNG__PARAMETRIZED_CALLBACK_H__

template <typename R, typename ...T> class PCallback;
template <typename R, typename ...T> class FpCallback;
template <typename R, typename C, typename ...T> class CpCallback;

// Template for wrapping Callbacks with parameter
template <typename R, typename ...T>
class PCallback
{
public:
    virtual ~PCallback() {}
    virtual PCallback<R,T...>* clone() const = 0;
    virtual R call(T...) = 0;
    R operator()(T... data) { return call(data...); }

    // Factory methods
    template <typename C>
    static PCallback * create(R(C::*clbk)(T...), C*obj) {
        return new CpCallback<R,C,T...>(clbk, obj);
    }
    static PCallback * create(R(*clbk)(T...)) {
        return new FpCallback<R,T...>(clbk);
    }
};

// Template for wrapping functions with paramenter
template <typename R, typename ...T>
class FpCallback : public PCallback<R,T...>
{
    R(*callback)(T...);
public:
    FpCallback(R(*clbk)(T...)) : callback(clbk) {}
    virtual FpCallback<R,T...>* clone() const { return new FpCallback<R,T...>(callback); }
    virtual R call(T... data) { return (*callback)(data...); }
};

// Template for wrapping class member functions with parameter
template <typename R, typename C, typename ...T>
class CpCallback : public PCallback<R,T...>
{
private:
    C* object;
    R (C::*callback)(T...);
public:
    CpCallback(R(C::*clbk)(T...), C*obj) : object(obj), callback(clbk) {}
    virtual CpCallback<R,C,T...>* clone() const { return new CpCallback<R,C,T...>(callback, object); }
    virtual R call(T... data) { return (object->*callback)(data...); }
};

#endif