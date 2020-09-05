#include <LedControl.h>

#define xPin A0
#define yPin A1
int buttonPin = 6;

// Buzzer
int buzzerPin = 7;

// LED Matrix
int dinPin = 8;
int csPin = 9;
int clkPin = 10;

// Joystick
int x_val;
int y_val;
int x = 1;
int y = 1;
int prev_x = 0;
int prev_y = 0;
int buttonVal;
unsigned int longPressThreshold = 1000;
bool lastState;
unsigned long lastTime;
unsigned long currTime;

byte world[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

// Tic-Tac-Toe / Knots + Crosses
int ticTacToeBoard[] = {0, 0, 0, 0, 0 , 0, 0, 0, 0};
int rowSize = 3;
int columnSize = 3;
bool blinkState = true;

// Game of Life
bool gol_board[8][8];
bool prev_board[8][8];
bool runningSim = false;
int numNeighbors;
int minusColumn;
int plusColumn;
int minusRow;
int plusRow;

LedControl matrix = LedControl(dinPin, clkPin, csPin, 1); // 1 b/c only using the one matrix

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

void loop() {
  if (!runningSim) {
    while (!runningSim) {
      blinkState = !blinkState;

      for (int i = 0; i < 8; i++) {
        matrix.setRow(0, i, world[i]);
      }
      int temp_y = map(y, 0, 7, 7, 0); // b/c array goes from 0...7 while bytes go 7...0 because you start at Least Significatn Byte which is on the right
      matrix.setLed(0, x, temp_y, blinkState);

      x_val = analogRead(xPin);
      if (x_val > 700 and x < 7) {
        x += 1;
      }
      else if (x_val < 300 and x > 0) {
        x -= 1;
      }

      y_val = analogRead(yPin);
      if (y_val > 700 and y < 7) {
        y += 1;
      }
      else if (y_val < 300 and y > 0) {
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

        bitWrite(world[x], y, (bitRead(world[x], y) ^ 1)); // toggle the bit at that loc
      }
      else if (buttonVal == HIGH) {
        lastState = HIGH;
      }

      prev_x = x;
      prev_y = y;

      delay(200);
    }
  }


  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      gol_board[x][y] = world[x][bitRead(world[x], map(y, 0, 7, 7, 0))];
      prev_board[x][y] = gol_board[x][y];
      bitWrite(world[x], map(y, 0, 7, 7, 0), gol_board[x][y]);
    }
  }

  for (int j = 0; j < 8; j++) {
    matrix.setRow(0, j, world[j]);
  }

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


      numNeighbors += prev_board[row - minusRow][column - minusColumn];
      numNeighbors += prev_board[row - minusRow][column];
      numNeighbors += prev_board[row - minusRow][column + plusColumn];

      numNeighbors += prev_board[row][column - minusColumn];
      //numNeighbors += prev_board[row][column]; // (self)
      numNeighbors += prev_board[row][column + plusColumn];

      numNeighbors += prev_board[row + plusRow][column - minusColumn];
      numNeighbors += prev_board[row + plusRow][column];
      numNeighbors += prev_board[row + plusRow][column + plusColumn];
      
      if (numNeighbors < 2 or numNeighbors > 3) { // if it doesn't have 2-3 neighbors
        gol_board[row][column] = false; // it dies
      } else { // else they continue living / are born
        gol_board[row][column] = true; // it is born/continues living
      }
    }
  }
  delay(200);
}
