#include "Display.h"

Display::Display(int numDevices) {
    totalSegments = numDevices;
}

void Display::init() {
    mx.begin();
}

void Display::clear() {
    mx.clear();
}

void Display::setLed(int row, int col, int status) {
    int correctedCol = (totalSegments * 8) - col - 1;
    mx.setPoint(row, correctedCol, status);
}

void Display::printChar(char currentChar, int startCol = 0) {
    for (int i = 0; i < 7; i++) {
        unsigned long cx = pgm_read_byte_near(font5x7 + ((currentChar - 0x20) * 8) + i);
        byte symbolSize = pgm_read_byte_near(font5x7 + ((currentChar - 0x20) * 8) + 7);
        for (int b = 7; b >= (7 - symbolSize + 1); b--) {
            byte bit = bitRead(cx, b);

            this->setLed(i, startCol + 7 - b, bit);
        }
    }
}

void Display::printText(char text[], int startCol = 0) {
    this->clear();
    int nextCol = startCol;
    for (int i = 0; i < strlen(text); i++) {
        this->printChar(text[i], nextCol);
        nextCol += 6;
    }
}

void Display::scrollText(char text[], int speed) {
    this->printText(text, scrollPos);
    delay(speed);
    scrollPos--;

    int textLen = strlen(text);
    int minScrollPos = textLen * -7;
    if (scrollPos < minScrollPos) {
        scrollPos = 32;
    }
}

void Display::resetScrollPosition() {
    scrollPos = 32;
}
