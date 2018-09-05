/**
 * @brief Simple example for usage
 * 
 * @file simple.ino
 * @author dinghino@gmail.com
 * @date 2018-09-05
 */

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
Timer timer;
A aObject;

void setup() {
    Serial.begin(9600);
    while(!Serial);
    // register the function to the timer giving it a 1s delay
    timer.every(1000, &func);
    // register aObject.callMe with 2.5s delay
    timer.every(2500, &A::callMe, &aObject);
    // This is a lambda function that will be called once in 5 seconds.
    timer.once<void>(5000, []() {
        Serial.println("Hello from lambda function.");
    });
}

void loop() {
    // tick the timer.
    timer.update();
}