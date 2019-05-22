#ifndef Buzzer_h
#define Buzzer_h

#include <Arduino.h>

class Buzzer {
private:
    int buzzerPin;

public:
    Buzzer(int pin);

    void beep(unsigned char delayms);

    void playTone(int tone, int duration);

    void playNote(char note, int duration);

    void playMusic(char notes[], int beats[], int tempo, int length);
};

#endif