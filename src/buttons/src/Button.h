#ifndef Button_h
#define Button_h

#include <Arduino.h>

#define PRESSED LOW
#define NOT_PRESSED HIGH

const unsigned long SHORT_PRESS_TIME  = 50;
const unsigned long LONG_PRESS_1_TIME = 1000;
const unsigned long LONG_PRESS_2_TIME = 4000;

class Button {
private:
    int id;

    int debounce = 10;
    unsigned long counter = 0;
    bool prevState = NOT_PRESSED;
    bool currentState;

    bool longPress_1_Noted = false;
    bool longPress_2_Noted = false;

    /** Methods */
    void log(char msg[]);

    bool isDebounce();
    bool isShortPressed();
    bool isLongPressed_1();
    bool isLongPressed_2();
    
    void signalLongPress_1();
    void signalLongPress_2();

    void handleShortPress();
    void handleLongPress_1();
    void handleLongPress_2();

    /** Callbacks */
    void (*_shortPressCallback)(int);
    void (*_longPress_1Callback)(int);
    void (*_longPress_2Callback)(int);
    void (*_longPress_1SignalCallback)(int);
    void (*_longPress_2SignalCallback)(int);
public:
    int pin;

    Button(int id, int pin);

    void checkState();

    // NOTE: callbacks need to be set or the program crashes
    void setShortPressCallback(void (*_shortPressCallback)(int));
    void setLongPress_1Callback(void (*_longPress_1Callback)(int));
    void setLongPress_2Callback(void (*_longPress_2Callback)(int));
    void setLongPress_1SignalCallback(void (*_longPress_1SignalCallback)(int));
    void setLongPress_2SignalCallback(void (*_longPress_2SignalCallback)(int));
};

#endif