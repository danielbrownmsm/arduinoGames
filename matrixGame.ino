#include <LedControl.h>

#define xPin A0
#define yPin A1
const int buttonPin = 6;

// Buzzer
const int buzzerPin = 7;

// LED Matrix
const int dinPin = 8;
const int csPin = 9;
const int clkPin = 10;

// Joystick
int x_val;
int y_val;
const int highThreshold = 800;
const int lowThreshold = 200;
int x = 1;
int y = 1;
int buttonVal;
const unsigned int longPressThreshold = 1000;
bool lastState;
unsigned long lastTime;
unsigned long currTime;
bool blinkState = true;

byte screen[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

// Game of Life
byte gol_board[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
byte prev_board[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
bool runningSim = false;
int numNeighbors;
int minusColumn;
int plusColumn;
int minusRow;
int plusRow;

LedControl matrix = LedControl(dinPin, clkPin, csPin, 1); // 1 b/c only using the one matrix

void flipDisplay(LedControl matrix, byte screen[]) {
  for (int i = 0; i < 8; i++) {
    matrix.setRow(0, i, screen[i]);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  matrix.shutdown(0, false);
  matrix.setIntensity(0, 0);
  matrix.clearDisplay(0);

  // really temp-temp
  x = 4;
  y = 4;
}

int byteToXY(int val) {
  return map(val, 0, 7, 7, 0);
}

void loop() {
  while (!runningSim) {
    blinkState = !blinkState; // toggle the blinking LED

    memcpy(screen, gol_board, sizeof(gol_board)); // move world data to screen
    flipDisplay(matrix, screen); // flip
    matrix.setLed(0, x, byteToXY(y), blinkState); // map b/c y is 0...7 while bytes is 7...0 (read from left to right)
    
    // get joystick vals
    x_val = analogRead(xPin);
    if (x_val > highThreshold and x < 7) {
      x += 1; // update accordingly
    } else if (x_val < lowThreshold and x > 0) {
      x -= 1;
    }

    y_val = analogRead(yPin);
    if (y_val > highThreshold and y < 7) {
      y += 1;
    } else if (lowThreshold < 300 and y > 0) {
      y -= 1;
    }

    buttonVal = digitalRead(buttonPin);
    if (buttonVal == LOW) {
      if (lastState == HIGH) {
        lastState = LOW;
        lastTime = millis();
      }
      currTime = millis();

      if (currTime - lastTime > longPressThreshold) { // if long press
        runningSim = true; // start the simulation
      }

      bitWrite(gol_board[x], y, (bitRead(gol_board[x], y) ^ 1)); // toggle the bit at that loc
    } else {
      lastState = HIGH;
    }
    delay(200);
  }

  flipDisplay(matrix, screen);

  for (int row = 0; row < 8; row++) { // for each row
    for (int column = 0; column < 8; column++ ) {
      int numNeighbors = 0;
      if (column == 0) {
        minusColumn = -7;
      } else {
        minusColumn = 1;
      }

      if (column == 7) {
        plusColumn = -7;
      } else {
        plusColumn = 1;
      }

      if (row == 7) {
        plusRow = -7;
      } else {
        plusRow = 1;
      }

      if (row == 0) {
        minusRow = -7;
      } else {
        minusRow = 1;
      }


      numNeighbors += bitRead(prev_board[row - minusRow], column - minusColumn);
      numNeighbors += bitRead(prev_board[row - minusRow], column);
      numNeighbors += bitRead(prev_board[row - minusRow], column + plusColumn);

      numNeighbors += bitRead(prev_board[row], column - minusColumn);
      numNeighbors += bitRead(prev_board[row], column + plusColumn);

      numNeighbors += bitRead(prev_board[row] + plusRow, column - minusColumn);
      numNeighbors += bitRead(prev_board[row] + plusRow, column);
      numNeighbors += bitRead(prev_board[row] + plusRow, column + plusColumn);

      if (numNeighbors > 2 and numNeighbors < 3) { // if it doesn't have 2-3 neighbors
        if (numNeighbors == 3) {
          bitSet(gol_board[row], column); // it is born if it has 3
        }
        // otherwise (for nested if) nothing happens (keeps state)
      } else { // else they continue living / are born
        bitClear(gol_board[row], column); // it dies
      }
    }
  }
  delay(200);
}
