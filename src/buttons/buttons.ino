#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "src/Button.h"

// Button 1 messages
#define RADIO_MSG_BUTTON_1_PRESS 0
#define RADIO_MSG_BUTTON_1_LONG_PRESS_1_SIGNAL 1
#define RADIO_MSG_BUTTON_1_LONG_PRESS_2_SIGNAL 2
#define RADIO_MSG_BUTTON_1_LONG_PRESS_1 3
#define RADIO_MSG_BUTTON_1_LONG_PRESS_2 4
// Button 2 messages
#define RADIO_MSG_BUTTON_2_PRESS 5
#define RADIO_MSG_BUTTON_2_LONG_PRESS_1_SIGNAL 6
#define RADIO_MSG_BUTTON_2_LONG_PRESS_2_SIGNAL 7
#define RADIO_MSG_BUTTON_2_LONG_PRESS_1 8
#define RADIO_MSG_BUTTON_2_LONG_PRESS_2 9

#define RADIO_MSG_HEARTBEAT 10

/** Radio variables */
RF24 radio(10, 8); // CE, CSN
const byte address[6] = "dsp001";
unsigned long lastHb = 0;

/** Buttons variables */
Button button1 = Button(1, 2);
Button button2 = Button(2, 3);

void setup() {
    Serial.begin(9600);
    Serial.print("Ready");
    Serial.println();
    
    // Setup radio
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();

    // Setup buttons
    pinMode(button1.pin, INPUT_PULLUP);
    pinMode(button2.pin, INPUT_PULLUP);

    button1.setShortPressCallback(onButtonShortPress);
    button1.setLongPress_1Callback(onButtonLongPress_1);
    button1.setLongPress_2Callback(onButtonLongPress_2);
    button1.setLongPress_1SignalCallback(onButtonLongPress_1_Signal);
    button1.setLongPress_2SignalCallback(onButtonLongPress_2_Signal);

    button2.setShortPressCallback(onButtonShortPress);
    button2.setLongPress_1Callback(onButtonLongPress_1);
    button2.setLongPress_2Callback(onButtonLongPress_2);
    button2.setLongPress_1SignalCallback(onButtonLongPress_1_Signal);
    button2.setLongPress_2SignalCallback(onButtonLongPress_2_Signal);
}

void loop() {
    button1.checkState();
    button2.checkState();
    sendHeartbeat();
}

/** Radio handler */
void sendRadioMessage(int msgId) {
    if(msgId > 0xff) return; 
    
    char buff[2]; // one byte of data max.. and null termination
    itoa(msgId, (char*)&buff, 10);
    radio.write(&buff, sizeof(buff));
}

void sendHeartbeat() {
    if (millis() - lastHb > 5000) {
        sendRadioMessage(RADIO_MSG_HEARTBEAT);
        lastHb = millis();
    }
}

/** Button handlers */
void onButtonShortPress(int id) {
    if (id == 1) sendRadioMessage(RADIO_MSG_BUTTON_1_PRESS);
    if (id == 2) sendRadioMessage(RADIO_MSG_BUTTON_2_PRESS);
}
void onButtonLongPress_1(int id) {
    if (id == 1) sendRadioMessage(RADIO_MSG_BUTTON_1_LONG_PRESS_1);
    if (id == 2) sendRadioMessage(RADIO_MSG_BUTTON_2_LONG_PRESS_1);
}
void onButtonLongPress_2(int id) {
    if (id == 1) sendRadioMessage(RADIO_MSG_BUTTON_1_LONG_PRESS_2);
    if (id == 2) sendRadioMessage(RADIO_MSG_BUTTON_2_LONG_PRESS_2);
}
void onButtonLongPress_1_Signal(int id) {
    if (id == 1) sendRadioMessage(RADIO_MSG_BUTTON_1_LONG_PRESS_1_SIGNAL);
    if (id == 2) sendRadioMessage(RADIO_MSG_BUTTON_2_LONG_PRESS_1_SIGNAL);
}
void onButtonLongPress_2_Signal(int id) {
    if (id == 1) sendRadioMessage(RADIO_MSG_BUTTON_1_LONG_PRESS_2_SIGNAL);
    if (id == 2) sendRadioMessage(RADIO_MSG_BUTTON_2_LONG_PRESS_2_SIGNAL);
}
