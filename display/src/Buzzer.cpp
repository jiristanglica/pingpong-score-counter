#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    this->buzzerPin = pin;
}

void Buzzer::beep(unsigned char delayms) {
    analogWrite(buzzerPin, 10);
    delay(delayms);
    analogWrite(buzzerPin, 0);
    delay(delayms);
}

void Buzzer::playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
      analogWrite(buzzerPin, 255);
      delayMicroseconds(tone);
      analogWrite(buzzerPin, 0);
      delayMicroseconds(tone);
  } 
}

void Buzzer::playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
        this->playTone(tones[i], duration);
    }   
  } 
}

void Buzzer::playMusic(char notes[], int beats[], int tempo, int length) {
    for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
            delay(beats[i] * tempo);
        } else {
            this->playNote(notes[i], beats[i] * tempo);
        }
        delay(tempo / 2);
    }
}