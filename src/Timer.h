#ifndef __DNG__CALLBACK_H_
#define __DNG__CALLBACK_H_

class Timer
{
    typedef std::function<void()> Callable;

    unsigned int _id;
    static unsigned int CREATED;
    // actual callable that will be executed when requested
    Callable callback;

  public:
    Timer(Callable clbk, unsigned long d, bool r);

    // Template constructor for class member functions
    template <class C>
    Timer(void (C::*clbk)(), C *obj, unsigned long d, bool r)
        : Timer(std::bind(clbk, obj), d, r) {}

    virtual ~Timer() {}

    virtual void operator()();
    virtual void call();

    inline bool exists() const { return callback != nullptr; }

    inline const unsigned int  id()       const { return _id; }

    inline const bool          repeat()   const { return bRepeat; }
    inline const void          repeat(bool r)   { return bRepeat = r; }

    inline const bool          called()   const { return bCalled; }

    inline const unsigned long getDelay() const { return ulDelay; }

    inline const unsigned long lastCall() const { return ulLastCall; }

protected:
    friend class TicToc;

    bool bCalled;
    bool bRepeat;
    unsigned long ulDelay;
    unsigned long ulLastCall;
};

class ExtTimer : public Timer
{
    std::function<bool()> f_Until;
    std::function<bool()> f_OnlyIf;

  public:
    ExtTimer(std::function<void()> clbk, unsigned long d, bool r);

    // Template constructor for class member functions
    template <class C>
    ExtTimer(void (C::*clbk)(), C *obj, unsigned long d, bool r)
        : ExtTimer(std::bind(clbk, obj), d, r) {}

    ~ExtTimer() {}

    void call() override;
    /**
     * @brief Add an extra callback to check if the registered function should run
     * Allows to modify internal values that will trigger the callback to not
     * be called if the provided function returns true.
     * Can be useful to make automatic check on results of an action or if some
     * external data changes.
     * @param func check to run, cannot receive parameters and must return `bool`
     * @return ExtTimer& this object
     */
    ExtTimer &until(std::function<bool()> func);
    /**
     * @brief Provided function will determine if Timer will run or not
     * Can be used to temporary (or undefinitely) stop the execution of a callback
     * from external parameters.
     * @param func check to run, cannot receive parameters and must return `bool`
     * @return ExtTimer& this object
     */
    ExtTimer &onlyIf(std::function<bool()> func);
    /**
     * @brief Call the registered function instantly (regadless of time and conditions)
     * Usually used to run upon callback registration without having to wait for
     * the set delay
     * @return ExtTimer& this object
     */
    ExtTimer &run();
};

#endif