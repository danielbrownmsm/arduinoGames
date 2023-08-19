#include <LedControl.h>
#include "Config.h"

// create the matrix object
LedControl matrix = LedControl(dinPin, clkPin, csPin, 1); // the '1' is because there's only one matrix

void setup() {
    // for debugging purposes
    #ifdef DEBUG_ON
    Serial.begin(9600);
    #endif

    // set the joystick pinmodes
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);

    // set the button pin to use the internal pullup resistor of the Arduino
    pinMode(buttonPin, INPUT_PULLUP);

    // set the buzzer to output
    pinMode(buzzerPin, OUTPUT);

    // gonna be honest don't know what I had these here for
    // pinMode(13, OUTPUT);
    // digitalWrite(13, LOW);

    // turn the matrix off
    matrix.shutdown(0, false); // 0 is adr of matrix, because LedControl can be used to control up to 8 matricies

    // set the LEDs to the lowest brightness setting to prevent drawing too much current
    // also they're just very bright
    matrix.setIntensity(0, 0);

    // clear the matrix
    matrix.clearDisplay(0);

    randomSeed(analogRead(5)); // seed PRNG with a floating pin
}

void loop() {

}

void displayNumber(int num) {
    //TODO
    // implementation should truncate 'num' to an int of 2 digits or something.
    // maybe a special display like NaN if it's too high?

    byte infinity[8] = { // (infinity symbol)
        B00000000,
        B00000000,
        B01100110,
        B10011001,
        B10011001,
        B01100110,
        B00000000,
        B00000000,
    }

    byte error[8] { // the little 'E' those little calculators display sometimes
        B00000000,
        B00000000,
        B01110000,
        B01000000,
        B01100000,
        B01000000,
        B01110000,
        B00000000,
    }

    return;
}

void canyonRunner() {
    // board setup
    byte board[8] = {
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
    }; // start with a full board and then we'll carve the canyon

    // game setup
    int score = 0;


    // main game loop
    while (1) {
        // carve out the canyon

        // handle movement
        
        // display board

        // handle collision

        // increase speed

        // increase score
        score++;
    }

    // display score
    displayNumber(score);
}

enum Operation {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION
};

int selectNumber() {
    // have player select first number (select tens digit, then ones digit)
    //TODO
    return 0;
}

void calculator() {
    // setup
    bool quit = false;
    int numOne = 0;
    int numTwo = 0;
    Operation operation;


    // main loop
    while (!quit) {
        // display the prompt for the 1st number
        //TODO

        // get the first number
        numOne = selectNumber();

        // display prompt for operation
        //TODO
        //select operation
        //TODO

        // display prompt for 2nd number
        //TODO

        // get the second number
        numTwo = selectNumber();

        if (operation == ADDITION) {
            displayNumber(numOne + numTwo);
        } else if (operation == SUBTRACTION) {
            displayNumber(numOne - numTwo);
        } else if (operation == MULTIPLICATION) {
            displayNumber(numOne * numTwo);
        } else if (operation == DIVISION) {
            displayNumber(numOne / numTwo);
        } else {
            // throw an error or something
            //TODO
        }

        // special logic we might do:
        // if player clicks enter then go to first step, but if player does right stick then set first number to result of calculation and go to operation select
    }
}

void guessingGame() {
    // setup
    bool guessed = false;
    int guesses = 0;

    int randNum = random.randint(1, 99);
    int playerGuess;

    // main game loop
    while (!guessed) {
        // have player select a number
        playerGuess = selectNumber();

        // if the player guessed right
        if (playerGuess == randNum) {
            // then the player has won!
            guessed = true;
        } else if (playerGuess < randNum) {
            // increase the score
            guesses++;

            // display the up arrow to tell the player to guess higher
            //TODO
        } else {
            // increase the score
            guesses++;

            // display down arrow
            //TODO
        }
    }

    // display the score
    displayNumber(guesses);
    wait();
}



block stack
21/blackjack
coin flip
tron
battleship?
google dino game
tetris
options menu
reaction timer
pinball
snake
paint
yahtzee?
timer?????
poker?
pokemon???
unblock me
tic-tac-toe (knoghts and crosses)
breakout (Atari)
microtransactions
John Conways GoL
maze
RPG???
some music making thing
support for writing programs on the console
pong
flappy bird
crossy road
simon says (memory game)
a screensaver (why the heck would you want this tho?)
rythym game (ex beatsaber)
racing game (obviously 2D)
some random dice game (what is that suppposed to mean? idk)
bowling
movies (stored in EEPROM) (how and why the heck? it's literally an 8x8 matrix and all one color with only basic sound support. If you really want to waste your time doing this then go ahead but until then)
a platformer
aliens (galaga)
a clicker
temple-run-but-not
connect-4
checkers???
gravity-guy-but-not?
party games (the heck does that mean?)