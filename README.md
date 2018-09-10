# TicToc
A generic timer for arduino program with conditional callbacks

TicToc allows you to avoid having to call `millis()` and make time comparison for everything you need to call from time to time and without using delays.

#### A little disclaimer
I'm not an expert C++ developer and this library is still in its infancy, meaning that there could be bugs, memory leaks and stuff that I didn't notice.

I'm currently testing and developing it while using it to develop a bigger project of mine so, while the current basic API of the library should remain the same I wouldn't consider it ready to be properly used.

Anyhow feel free to pick it up and integrate it in your projects and I'm always open for suggestions and PR if they make sense and improve readability or functionality.

When I think it'll be ready it will become `v1.0` and will most probably publish it on the `PlatformIO` library store for those who use that as an IDE.

## Basic example
Some examples can be found in the `examples` folder but for the TLDR version a super short example
```c++
#include <TicToc.h>

// create the TicToc manager
TicToc timer;

void HelloWorld()
{
    Serial.println("Hello world");
}

void setup()
{
    // Will fire the function JUST once AFTER 1 second from the registration
    timer.once(1000, &HelloWorld);
    // This will do it EVERY second starting from the registration
    timer.every(3000, &HelloWorld);
}


void loop()
{
    // ticks the timer and execute when needed
    timer.update();
}
```

A more advanced use can be achieved with the callbacks registered with `every`, like

```c++

// ...
bool shouldRun() { return false; }

void setup()
{
    // This will put a safeguard from the execution of the callback, that will
    // be called only if (duh) the return value of the provided function is
    // evaluated true
    timer.every(2000, &HelloWorld).onlyIf(&shouldRun);
}

```

Another cool example could be something like

```c++

// A generic class that can do something and holds state
class A
{
    public:
    int counter;
    void increment();
};

void logIt();

TicToc timer;
A aObject;

void loop()
{
    // register a member function as callback
    timer.every(2500, &A::increment, &aObject);
    // register a function that will be called only on multiple conditions
    timer.every(1000, &logit)
        // will print only 3, 6, 9, ...
        .onlyIf([&]() { return aObject.count % 3 == 0; })
        // ...up to 21, then it will stop
        .until([&]() { return aObject.count <= 21; });
}

void logIt()
{
    Serial.print("Value is now at ");
    Serial.println(aObject.counter);
}

void A::increment()
{
    Serial.print("Incremented to: ");
    Serial.println(++counter);
}
```
