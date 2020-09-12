#include <LedControl.h>

#define xPin A0
#define yPin A1
const int buttonPin = 6;

// Buzzer (not currently used)
const int buzzerPin = 7;

// LED Matrix
const int dinPin = 8;
const int csPin = 9;
const int clkPin = 10;

// Joystick + random
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

// blackjack
int p_total = 0;
int c_total = 0;
int c_threshold = 17;
long newCard = 0;
bool hit = false;
bool decided = false;

// symbols
byte plusSign[8] = {B00000000, B00011000, B00011000, B01111110, B01111110, B00011000, B00011000, B00000000}; // plus symbol
byte minusSign[8] = {B00000000, B00000000, B00000000, B01111110, B01111110, B00000000, B00000000, B00000000}; // minus symbol
byte happy[8] = {B00000000, B01100110, B01100110, B00000000, B00000000, B01000010, B00111100, B00000000}; // happy face
byte sad[8] = {B00000000, B01100110, B01100110, B00000000, B00000000, B00111100, B01000010, B00000000}; // sad face

// tower stack blocks
byte lastLevel = B011111110;
byte newLevel;
byte tower[7] = {};
int write_loc = 0;
int read_loc = 0;
int tower_x = 0;
int tower_y = 0;
int direction = 0;

// numerals
byte digits[10][5] = {{B00000111, B00000101, B00000101, B00000101, B00000111},
                    {B00000001, B00000011, B00000001, B00000001, B00000001}, 
                    {B00000111, B00000001, B00000111, B00000100, B00000111}, 
                    {B00000111, B00000001, B00000111, B00000001, B00000111}, 
                    {B00000101, B00000101, B00000111, B00000001, B00000001}, 
                    {B00000111, B00000100, B00000111, B00000001, B00000111}, 
                    {B00000111, B00000100, B00000111, B00000101, B00000111}, 
                    {B00000111, B00000001, B00000010, B00000010, B00000100}, 
                    {B00000111, B00000101, B00000111, B00000101, B00000111}, 
                    {B00000111, B00000101, B00000101, B00000101, B00000111}}; // everything at it's index

// menu
String game = "21";

LedControl matrix = LedControl(dinPin, clkPin, csPin, 1); // 1 b/c only using the one matrix

void setup() {
  Serial.begin(9600);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  matrix.shutdown(0, false); // 0 is adr of matrix, because LedControl can be used to control up to 8 matrix-es
  matrix.setIntensity(0, 0); // b/c LEDs can be _very_ brigt (and also draw current)
  matrix.clearDisplay(0);

  randomSeed(analogRead(5)); // seed PRNG with floating pin (unless need be A5? (not to be confused with @55/A$$))
}

int byteToXY(int val) { // really I would say Cartesian but who has space to write that?
  return map(val, 0, 7, 7, 0);
}

void loop() {
  if (game == "gol") {
    while (!runningSim) { // before we are running the game and are still setting up
      blinkState = !blinkState; // toggle the blinking LED

      memcpy(screen, gol_board, sizeof(gol_board)); // move world data to screen
      for (int i = 0; i < 8; i++) {
        matrix.setRow(0, i, screen[i]); // flip
      }
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
      } else if (y_val < lowThreshold and y > 0) {
        y -= 1;
      }

      buttonVal = digitalRead(buttonPin);
      if (buttonVal == LOW) { // this is for figuring out if it is a long or short press
        if (lastState == HIGH) {
          lastState = LOW;
          lastTime = millis();
        }
        currTime = millis();

        if (currTime - lastTime > longPressThreshold) { // if long press
          runningSim = true; // start the simulation
          continue; // we don't want to toggle the bit at the location of the long press
        }

        bitWrite(gol_board[x], y, (bitRead(gol_board[x], y) ^ 1)); // toggle the bit at that loc
      } else {
        lastState = HIGH;
      }
      delay(200);
    }

    memcpy(screen, gol_board, sizeof(gol_board)); // move world data to screen
    memcpy(prev_board, gol_board, sizeof(gol_board)); // archive world data (basically not really)
    for (int i = 0; i < 8; i++) { // flip
      matrix.setRow(0, i, screen[i]);
    }
    
    for (int row = 0; row < 8; row++) { // for each row
      for (int column = 0; column < 8; column++ ) {
        int numNeighbors = 0;

        /** plus/minus column/row is for edges so it wraps around. I may have not implemented it right though.
         * World is supposed to be a torus
         */
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
              
        if (numNeighbors > 1 and numNeighbors < 4) { // if it has 2-3 neighbors
          if (numNeighbors == 3) {
            bitSet(gol_board[row], column); // it is born if it has 3
          }
          // otherwise (for nested if) nothing happens (keeps state)
        } else { // else they continue living / are born
          bitClear(gol_board[row], column); // it dies
        }
      }
    }
    delay(500);
  }
  else if (game == "21") {
    // do blackjack here
    matrix.clearDisplay(0);
    p_total = random(1, 10);
    hit = true;
    decided = false;

    while (hit) {
      p_total += random(1, 10);
      int onesDigit = p_total % 10;
      int tensDigit = (p_total / 10) % 10; // because result is int it removes stuff after decimal
      for (int i = 0; i < 5; i++) { // b/c we only have 5 rows of number to display
        matrix.setRow(0, i+1, (digits[tensDigit][i] << 4) | digits[onesDigit][i]); // this _should_ work. Maybe. Honestly tho it prob won't
      }

      decided = false;
      while(!decided) {
        decided = !digitalRead(buttonPin);
        delay(300);
      }
      decided = false;
      
      // check hit or stay
      while (!decided) {
        y_val = analogRead(yPin);
        if (y_val > highThreshold and y < 1) {
          y += 1; // update accordingly
        } else if (y_val < lowThreshold and y > 0) {
          y -= 1;
        }

        if (y == 1) {
          for (int i = 0; i < 8; i++) {
            matrix.setRow(0, i, plusSign[i]);
          }
        } else {
          for (int i = 0; i < 8; i++) {
            matrix.setRow(0, i, minusSign[i]);
          }
        }

        decided = !digitalRead(buttonPin); // dang you can write stuff short!
        hit = (y == 1); // if y is one then yes hit otherwise no
      }
    }

    c_total = random(1, 10) + random(1, 10);
    while (c_total < c_threshold) {
      c_total += random(1, 10); // computer hits if below its threshold
    }

    if ((c_total > 21 || p_total > c_total) && p_total < 22) {
      for (int i = 0; i < 8; i++) {
        matrix.setRow(0, i, happy[i]);
      }
    } else { 
      for (int i = 0; i < 8; i++) {
        matrix.setRow(0, i, sad[i]);
      }
    }
    delay(2000);
  }
  else if (game == "stac") {
    if(!digitalRead(buttonPin)) { // if button press
      new_level = tower[tower_y] & tower[tower_y + 1]; // new level is then what the heck am I doing?
    }

    if (direction == 1) {
      if (!bitRead(tower[tower_y], byteToXY(0))) { // if we're not at the edge
        tower[tower_y] = tower[tower_y] << 1; // move it left
      } else { // else we're at the edge
        direction = 0;
      }
    } else {
      if (!bitRead(tower[tower_y], byteToXY(7))) { // and then repeat everything
        tower[tower_y] = tower[tower_y] >> 1;
      } else {
        direction = 1;
      }
    }
    delay(100);
  }
}
