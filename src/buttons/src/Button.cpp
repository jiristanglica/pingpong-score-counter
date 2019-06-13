#include "Button.h"

Button::Button(int id, int pin) {
    this->id = id;
    this->pin = pin;
}

void Button::log(char msg[]) {
    Serial.print("[Button ");
    Serial.print(this->id);
    Serial.print("] ");
    Serial.print(msg);
    Serial.println();
}

/** Helper methods */
bool Button::isDebounce() {
    return (millis() - this->counter <= this->debounce);
}

bool Button::isShortPressed() {
    return (millis() - this->counter >= SHORT_PRESS_TIME) && !(millis() - this->counter >= LONG_PRESS_1_TIME);
}
bool Button::isLongPressed_1() {
    return (millis() - this->counter >= LONG_PRESS_1_TIME) && !(millis() - this->counter >= LONG_PRESS_2_TIME);
}
bool Button::isLongPressed_2() {
    return (millis() - this->counter >= LONG_PRESS_2_TIME);
}

/** Button state checking */
void Button::checkState() {
    // Read the current state of the button
    this->currentState = digitalRead(this->pin);

    if (this->currentState == this->prevState) {
        // Signals for long presses
        if (this->currentState == PRESSED && this->isLongPressed_1() && !this->longPress_1_Noted) {
            this->signalLongPress_1();
        }
        if (this->currentState == PRESSED && this->isLongPressed_2() && !this->longPress_2_Noted) {
            this->signalLongPress_2();
        }
        return;
    }

    if (this->currentState == PRESSED) {
        // New press event occured, record when button went down
        this->counter = millis();
    }
    else if(!this->isDebounce()) {
        // No longer pressed, handle based on how long was it pressed        
        if (this->isShortPressed()) {
            this->handleShortPress();
        }
        else if (this->isLongPressed_1()) {
            this->handleLongPress_1();
        }
        else if (this->isLongPressed_2()) {
            this->handleLongPress_2();
        }
    }
    this->prevState = this->currentState;
}

/** Press signals */
void Button::signalLongPress_1() {
    this->log("* Long Press 1 Signal");
    this->longPress_1_Noted = true;
    this->_longPress_1SignalCallback(this->id);
}
void Button::signalLongPress_2() {
    this->log("* Long Press 2 Signal");
    this->longPress_2_Noted = true;
    this->_longPress_2SignalCallback(this->id);
}

/** Press handlers */
void Button::handleShortPress() {
    this->log("Short Press");
    this->_shortPressCallback(this->id);
}
void Button::handleLongPress_1() {
    this->longPress_1_Noted = false;
    this->longPress_2_Noted = false;
    this->log("Long Press 1");
    this->_longPress_1Callback(this->id);
}
void Button::handleLongPress_2() {
    this->longPress_1_Noted = false;
    this->longPress_2_Noted = false;
    this->log("Long Press 2");
    this->_longPress_2Callback(this->id);
}

/** Callbacks setters */
void Button::setShortPressCallback(void (*callback)(int))
{
    this->_shortPressCallback = callback;
}
void Button::setLongPress_1Callback(void (*callback)(int))
{
    this->_longPress_1Callback = callback;
}
void Button::setLongPress_2Callback(void (*callback)(int))
{
    this->_longPress_2Callback = callback;
}
void Button::setLongPress_1SignalCallback(void (*callback)(int))
{
    this->_longPress_1SignalCallback = callback;
}
void Button::setLongPress_2SignalCallback(void (*callback)(int))
{
    this->_longPress_2SignalCallback = callback;
}