/**
 * @brief Simple example for usage
 * 
 * @file simple.ino
 * @author dinghino@gmail.com
 * @date 2018-09-05
 */

#include <Arduino.h>
#include <Timer.h>

// Function to be called
void func() {
    Serial.println("Hello World from function");
}

// a class that has a member that needs to be called every second
class A {
public:
    int callMe() {
        Serial.println("Hello World from A::callMe");
        return 1;
    }
};


// Create a timer object
Timer timer
A aObject;

void setup() {
    // register the function to the timer giving it a 1s delay
    timer.setInterval(func, 1000);
    // register aObject.callMe with 2.5s delay
    timer.setInterval(&A::callMe, &aObject, 2500);
    // This is registered as timeout, so will be called once.
    timer.setTimeout([]() {
        Serial.println("Hello from lambda function.");
    }, 5000);
}

void update() {
    // tick the timer.
    timer.update();
}