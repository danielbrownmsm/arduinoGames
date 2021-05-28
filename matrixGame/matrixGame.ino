#include <LedControl.h>

// PINS
#define xPin A0
#define yPin A1
#define buttonPin = 6;

#define buzzerPin = 7;

#define dinPin = 8;
#define csPin = 9;
#define clkPin = 10;

// CONTROLS
int x_val; // x value of the joystick
int y_val; // y value of the joystick
int buttonVal; // if the button is pressed or not

const unsigned int longPressThreshold = 1000; // threshold for a long button press
bool lastState; // previous state of the button (current minus 1)
unsigned long lastTime; // last time the button's state changed or something
unsigned long currTime; // current system time

// array to hold the state of the screen
byte screen[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

// actual LED matrix. the "1" is because we are only using one matrix/display
LedControl matrix = LedControl(dinPin, clkPin, csPin, 1);



void setup() {
  Serial.begin(9600);

  // set all the pinmodes
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // turn off the built-in LED (13 on the ATMega2560, may vary)
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  matrix.shutdown(0, false); // turn the matrix on. the "0" is the index of the matrix (out of all the ones plugged in or something)
  matrix.setIntensity(0, 0); // set intensity to minimum (to lower brightness and lessen current draw)
  matrix.clearDisplay(0); // clear the display (of matrix #0)

  randomSeed(analogRead(5)); // seed PRNG with value from a floating pin
}

//TODO explain what this does
int byteToXY(int val) { // really I would say Cartesian but who has space to write that?
  return map(val, 0, 7, 7, 0);
}

void loop() {
}
