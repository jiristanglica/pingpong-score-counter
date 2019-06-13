#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include "src/Display.h"
#include "src/Buzzer.h"

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

/** Buzzer variables */
Buzzer buzzer(3);

/** Radio varaibles */
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "dsp001";
unsigned long lastHb = 0;
bool txLost = false;

/** Display variables */
Display disp = Display(4);

/** Display messages */
const char msgNewGame[] =    " New Game       ";  // 1
const char msgPlayer1Won[] = " Player 1 wins! ";  // 2
const char msgPlayer2Won[] = " Player 2 wins! ";  // 3
/** Display messages helper variables */
bool isTextScrolling = false;
unsigned long textScrollingTimer = 0;
byte msgDisplayed = 1;

/** Players variables */
typedef struct Players {
    int id;
    int score = 0;
    bool isServing = false;

    String scoreString() {
        char cstr[3]; // 2 chars + null termination
        itoa(this->score, cstr, 10);
        return cstr;
    }
    char scoreChar1() {
        String str = this->scoreString();
        if (str[1]) {
            return str[0];
        }
        return '0';
    }
    char scoreChar2() {
        String str = this->scoreString();
        if (str[1]) {
            return str[1];
        }
        return str[0];
    }
    void renderScore() {
        int renderOffset = 0;
        if (this->id == 2) {
            renderOffset = 19;
        }
        disp.printChar(this->scoreChar1(), renderOffset + 1);
        disp.printChar(this->scoreChar2(), renderOffset + 7);
        disp.setLed(7, renderOffset + 5, this->isServing);
        disp.setLed(7, renderOffset + 6, this->isServing);
        disp.setLed(7, renderOffset + 7, this->isServing);
    }
} Player;
Player player1;
Player player2;

/** Gameplay variables */
bool isGameOver = false;
int servingCounter = 0;

void setup()
{
    Serial.begin(9600);
    Serial.print("Ready");
    Serial.println();
    
    // Setup display
    disp.init();
    
    // Setup radio
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    
     // Setup players
    player1.id = 1;
    player2.id = 2;

    playStartupTone();
    
    resetGame();
}

void loop()
{
    checkHeartbeat();
    if (radio.available()) {
        char text[3] = "";
        radio.read(&text, sizeof(text));
        handleRadioMessage(text);
    }
    if (isTextScrolling) {
        if (msgDisplayed == 1) {
            disp.scrollText(msgNewGame, 35);
        } else if (msgDisplayed == 2) {
            disp.scrollText(msgPlayer1Won, 35);
        } else if (msgDisplayed == 3) {
            disp.scrollText(msgPlayer2Won, 35);
        } else {
            resetScrollMessage();
        }
        if ((unsigned long)(millis() - textScrollingTimer) >= 6500) {
            resetScrollMessage();
        }
    }
}

/** Buzzer functions */
void signalLongPress1() {
    buzzer.beep(75);
    delay(50);
    buzzer.beep(75);  
}
void signalLongPress2() {
    buzzer.beep(100);
    delay(20);
    buzzer.beep(100);
    delay(20);
    buzzer.beep(100);
    delay(20);
    buzzer.beep(100);
}
void playStartupTone() {
    int length = 5;
    char notes[] = "eee C";
    int beats[] = { 2, 2, 2, 2, 5 }; 
    int tempo = 60;

    buzzer.playMusic(notes, beats, tempo, length);

    delay(200);
}
void playWinTone() {
    int length = 12;
    char notes[] = "cdefgabC eb";
    int beats[] = { 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 6 }; 
    int tempo = 60;

    buzzer.playMusic(notes, beats, tempo, length);
}

/** Radio handler */
void handleRadioMessage(char msgChars[]) {
    msgChars[2] = '\0'; // failing atoi is undefined behaviour.. tryihg to avoid.. TODO: is strtol possible ?
    // COMMENT: using string commands would be a bit more robust solution. fex: "1" instead of 1
    // If using integer commands, I would convert this to switch-case
    int msgId = atoi(msgChars);
    if (msgId == RADIO_MSG_BUTTON_1_PRESS) addPlayerScore(1);
    if (msgId == RADIO_MSG_BUTTON_1_LONG_PRESS_1_SIGNAL) signalLongPress1();
    if (msgId == RADIO_MSG_BUTTON_1_LONG_PRESS_2_SIGNAL) signalLongPress2();
    if (msgId == RADIO_MSG_BUTTON_1_LONG_PRESS_1) removePlayerScore(1);
    if (msgId == RADIO_MSG_BUTTON_1_LONG_PRESS_2) resetGame();

    if (msgId == RADIO_MSG_BUTTON_2_PRESS) addPlayerScore(2);
    if (msgId == RADIO_MSG_BUTTON_2_LONG_PRESS_1_SIGNAL) signalLongPress1();
    if (msgId == RADIO_MSG_BUTTON_2_LONG_PRESS_2_SIGNAL) signalLongPress2();
    if (msgId == RADIO_MSG_BUTTON_2_LONG_PRESS_1) removePlayerScore(2);
    if (msgId == RADIO_MSG_BUTTON_2_LONG_PRESS_2) resetGame();

    if (msgId == RADIO_MSG_HEARTBEAT) handleHeartbeat(); 
}

void handleHeartbeat() {
    lastHb = millis();
    if (txLost == true) {
        txLost = false;
        disp.clear();
        displayPlayerScores();
    }
}
void checkHeartbeat() {
    if (txLost == false && millis() - lastHb > 11000) {
        txLost = true;
        disp.printText("NO TX");
    }
}

/** Display functions */
void displayScrollMessage(byte msgId) {
    msgDisplayed = msgId;
    isTextScrolling = true;
    textScrollingTimer = millis();
}
void resetScrollMessage() {
    isTextScrolling = false;
    disp.clear();
    disp.resetScrollPosition();
    displayPlayerScores();
}
void displayPlayerScores() {
    // Score delimiter
    disp.setLed(2, 15, 1);
    disp.setLed(2, 16, 1);
    disp.setLed(5, 15, 1);
    disp.setLed(5, 16, 1);

    // Score numbers
    player1.renderScore();
    player2.renderScore();
}


/** Players logic */
void addPlayerScore(int playerId) {
    if (isTextScrolling) return;
    
    // If game has ended, use a short press as a shortcut to reset the game
    if (isGameOver) {
        resetGame();
        return;
    }

    buzzer.beep(100);
    if (player1.score == 0 && !player1.isServing && player2.score == 0 && !player2.isServing) {
        // New game, first point determines who is serving first
        if (playerId == 1) {
            player1.isServing = true;
            player1.renderScore();
        }
        if (playerId == 2) {
            player2.isServing = true;
            player2.renderScore();
        }
        return;
    }
    if (playerId == 1) {
        player1.score++;
    }
    else if (playerId == 2) {
        player2.score++;
    }
    servingCounter++;
    checkServing();
    checkWin();
}
void removePlayerScore(int playerId) {
    if (playerId == 1 && player1.score > 0) {
        player1.score--;
        servingCounter--;
        checkServing();
    }
    if (playerId == 2 && player2.score > 0) {
        player2.score--;
        servingCounter--;
        checkServing();
    }
}
void checkServing() {
    if (servingCounter == 2) {
        player1.isServing = !player1.isServing;
        player2.isServing = !player2.isServing;
        servingCounter = 0;
    }
    if (servingCounter < 0) {
        player1.isServing = !player1.isServing;
        player2.isServing = !player2.isServing;
        servingCounter = 1;
    }
    displayPlayerScores();
}
void checkWin() {
    if (player1.score >= 11 || player2.score >= 11) {
        if ((player1.score - player2.score) >= 2) {
            Serial.println("Player 1 win!");
            isGameOver = true;
            delay(1000);
            // displayScrollMessage(2);
            playWinTone();
        }
        if ((player2.score - player1.score) >= 2) {
            Serial.println("Player 2 win!");
            isGameOver = true;
            delay(1000);
            // displayScrollMessage(3);
            playWinTone();
        }
    }  
}
void resetGame() {
    isGameOver = false;
    servingCounter = 0;
    player1.score = 0;
    player1.isServing = false;
    player2.score = 0;
    player2.isServing = false;
    displayPlayerScores();
    displayScrollMessage(1);
    buzzer.beep(500);
}

