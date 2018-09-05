/**
 * @brief Simple example for usage
 * 
 * @file simple.ino
 * @author dinghino@gmail.com
 * @date 2018-09-05
 */

#include <Arduino.h>
#include <Timer.h>

// Function to be called every second
void func() {
    Serial.println("Hello World from function");
}

// a class that has a member that needs to be called every second
class A {
public:
    int callEverySecond() {
        Serial.println("Hello World from A::callEverySecond");
        return 1;
    }
};



// Create a timer object that will call everything passed every 1000 ms (1 second)
Timer timer(1000);
A aObject;

void setup() {
    // register the function to the timer
    timer.addCallback(func);
    // register aObject.callEverySecond
    timer.addCallback(&A::callEverySecond, &a);
}

void update() {
    // tick the timer
    timer.update();
}