#include <LedControl.h>

// PINS
#define xPin A0
#define yPin A1
#define buttonPin A2 // !! NOTE !! b/c of the wiring and stuff, the analogRead will read some random values (seed PRNG, anyone?) when unpressed, and ~1020 when pressed (a check for >1010 should work just fine))

#define buzzerPin 8

#define dinPin 5
#define csPin 6
#define clkPin 7

// CONTROLS
//int x_val; // x value of the joystick
//int y_val; // y value of the joystick
//int buttonVal; // if the button is pressed or not

//const unsigned int longPressThreshold = 1000; // threshold for a long button press
//bool lastState; // previous state of the button (current minus 1)
//unsigned long lastTime; // last time the button's state changed or something
//unsigned long currTime; // current system time

// CONSTANT ARRAY SCREEN THINGS
const byte digits[10][5] = {
  {B00000111, B00000101, B00000101, B00000101, B00000111},
  {B00000001, B00000011, B00000001, B00000001, B00000001}, 
  {B00000111, B00000001, B00000111, B00000100, B00000111}, 
  {B00000111, B00000001, B00000111, B00000001, B00000111}, 
  {B00000101, B00000101, B00000111, B00000001, B00000001}, 
  {B00000111, B00000100, B00000111, B00000001, B00000111}, 
  {B00000111, B00000100, B00000111, B00000101, B00000111}, 
  {B00000111, B00000001, B00000010, B00000010, B00000100}, 
  {B00000111, B00000101, B00000111, B00000101, B00000111}, 
  {B00000111, B00000101, B00000101, B00000101, B00000111}
}; // everything at it's index
byte win[8] = { // maybe???
  B00111100,
  B11111111,
  B10111101,
  B10111101,
  B01111110,
  B00011000,
  B00011000,
  B00111100
};
byte lose[8] = { // (I'ts an "X")
  B11000011,
  B11100111,
  B01111110,
  B00111100,
  B00111100,
  B01111110,
  B11100111,
  B11000011,
};

// actual LED matrix. the "1" is because we are only using one matrix/display
LedControl matrix = LedControl(dinPin, clkPin, csPin, 1);
// !! NOTE !! (0, 0) is the lower right-hand corner of the display. Draw accordingly

// drawLine(x1, y1, x2, y2)
// setLed(x, y)
// drawRect(x, y, width, height)
// drawCircle(x, y, radius)
// blitSprite(sprite, x, y)
// displayDigit(number, x=0, y=0) // set optional params to be correct vals
// button.onPress / onRelease / onPressRelease / whileHeld / whileReleased / onLongPress
// joy.whenInRange / whileInRange / whenNotInRange / whileNotInRange
// double-buffered screen


//TODO look into using char instead of int to save a byte of space everywhere if less than 127/255 for unsigned is needed
//TODO implement a difficulty setting

void setup() {
  Serial.begin(9600);

  // set all the pinmodes
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // turn off the built-in LED (13 on the ATMega2560, may vary)
  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);

  matrix.shutdown(0, false); // turn the matrix on. the "0" is the index of the matrix (out of all the ones plugged in or something)
  matrix.setIntensity(0, 0); // set intensity to minimum (to lower brightness and lessen current draw)
  matrix.clearDisplay(0); // clear the display (of matrix #0)

  randomSeed(analogRead(5)); // seed PRNG with value from a floating pin
}

// because binary numbers work with bit 0 all the way to the right and increase as you go left,
// but decimal numbers are opposite this, this just maps a binary to decimal number (based on digit-placement)
int byteToXY(int val) { // really I would say Cartesian but who has space to write that?
  return map(val, 0, 7, 7, 0);
}

// takes an array of bytes and writes them to the display
void blit(byte array[8]) {
  for (int row = 0; row < sizeof(array); row++) {
    matrix.setRow(0, row, array[row]);
  }
}

// prints a number (between 0 and 99) to the screen. This is non-blocking
void printDigits(int number) {
  int onesDigit = number % 10;
  int tensDigit = (number / 10) % 10; // because result is int it removes stuff after decimal
  for (int i = 0; i < 5; i++) { // b/c we only have 5 rows of number to display
    matrix.setRow(0, i+1, (digits[tensDigit][i] << 4) | digits[onesDigit][i]); // this _should_ work. Maybe. Honestly tho it prob won't
  }
}

// waits for a read on the button pin (so you can display things and wait for the player to continue)
void wait() {
  while (!digitalRead(buttonPin)) {
    delay(100);
  }
}

// clamps a value between a given minimum and maximum
int clamp(int val, int min, int max) {
  if (val < min) {
    return min;
  } else if (val > max) {
    return max;
  }
  return val;
}

void loop() {
  matrix.setRow(0, 0, B00000001);
  Serial.print("X: ");
  Serial.print(analogRead(xPin));
  Serial.print(" | Y: ");
  Serial.print(analogRead(yPin));
  Serial.print(" | SW: ");
  Serial.print(analogRead(buttonPin));
  Serial.print("\n");
  delay(300);

  towerStackBlocks();
  blackjack();
  canoyonRunner();
  guessingGame();

  //index = 0;
  //button.onPress(games[index].run());
  //CommandScheduler.run();
}

void towerStackBlocks() {
  byte tower[8] = {B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110};
  int direction = 1; // 1 for moving right, -1 for moving left
  int score = 0;
  bool lastButtonState = false; // used for
  bool buttonPress = false;     // buffering/toggling inputs

  while(true) {
    lastButtonState = buttonPress;
    buttonPress = digitalRead(buttonPin);
    
    blit(tower);
    if (bitRead(tower[0], 0) || bitRead(tower[0], 8)) { // if we've reached one of the edges
      direction *= -1; // invert the direction
    }

    tower[0] = ((direction > 0) ? (tower[0] >> 1) : (tower[0] << 1)); // shift the active level left or right, depending on direction
    if (digitalRead(buttonPin) && lastButtonState == false) { // if the button is pressed
      tower[1] &= tower[0]; // shave off the bits that missed

      // tower[6] is base
      // tower[0] is the moving level
      // only 7 items (0-6) b/c top one is always empty/blank/off
      for (int i = 6; i > 1; i--) { // move everything down a level
        tower[i] = tower[i-1];
      }
      tower[0] = tower[1]; // and stack it
      score++;
    }

    if (tower[0] == 0) { // lose condition
      printDigits(score);
      wait();
      return; // exit or something idk
    }

  }
}

void blackjack() {
  int cpuTotal = random(1, 10);
  if (cpuTotal < 17) {
    cpuTotal += random(1, 10);
  }

  int playerTotal = random(1, 10);
  printDigits(playerTotal);
  wait();

  bool playerStays = false;
  while (!playerStays) {
    //TODO handle inputs and logic
    if (digitalRead(buttonPin)) {
    }
  }

  if (cpuTotal > 21 || (playerTotal > cpuTotal && playerTotal < 22)) {
    blit(win);
  } else {
    blit(lose);
  }
  wait();
}

void dispTwoArrays(byte arrayOne[8], byte arrayTwo[8]) { // literally only a helper func for battleship
  for (int i = 0; i < 8; i++) {
    matrix.setRow(0, i, arrayOne[i] | arrayTwo[i]);
  }
}

void battleship() {
  // set up field
  byte player[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte playerMisses[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte playerHits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int playerTotal = 0;

  byte cpu[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte cpuMisses[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte cpuHits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int cpuTotal = 0;

  bool playerDonePlacing = false;
  bool cursorOn = false;
  int cursorX = 0;
  int cursorY = 0;

  /* PRV | CUR | VAL
   *  0  |  0  |  0
   *  0  |  L  | -1
   *  L  |  0  |  ???
   *  L  |  L  | -2
   *  L  |  R  |  ???
   *  0  |  R  |  1
   *  R  |  0  |  ???
   *  R  |  R  |  2
   *  R  |  L  |  ???
   */
  int lastXState = 0; // 0 is neutral, -1 is going from 0 to left, -2 is left, ???, 1 is going from 0 to right, 2 is right, ???
  int lastYState = 0; // same as above
  //Battleship playerShips
  while(!playerDonePlacing) {
    //TODO logic
  }

  //CPU placing time
  for (int shipNum = 0; shipNum < 4; shipNum++) {
    bool horizontal = random(1);
    int shipSize = random(1, 4);
    
    if (horizontal) {
      int shipX = random(7 - shipSize);
      int shipY = random(7);

      byte ship = ((int)pow(2, shipSize)) << shipX; // actual ship in actual loc
      if (ship & cpu[shipY]) { // if it collides with any other currently placed ship
        shipNum--; // try again
      }
    } else {
      int shipX = random(7);
      int shipY = random(7 - shipSize);

      for (int x = 0; x < shipY; x++) {
        //TODO
      }
    }
    
    cpuTotal += shipSize;
  }

  while (true) { // main game loop
    // player block
    bool playerDonePicking = false;
    int guessX = 0;
    int guessY = 0;

    //while (!playerDonePicking) {
      //TODO blinking and stuff (using dispTwoArrays or something)
      //TODO and also input handling
    //}

    
    if (bitRead(cpu[guessY], byteToXY(guessX))) {
      playerHits[guessY][guessX] = 1; //TODO change to bitWrite()
    } else {
      playerMisses[guessY][guessX] = 1;
    }

    // cpu block
    bool cpuValidGuess = false;
    while (!cpuValidGuess) { // try to get CPU to randomly guess (this might be really bad design idk)
      guessX = random(7); //TODO fix this like @JustinKleiber said (just have a list of guessable values, and pop a guess from it each time)
      guessY = random(7);

      if (!bitRead(cpuMisses[guessY], guessX)) {
        if (!bitRead(cpuHits[guessY], guessX)) {
          cpuValidGuess = true;
        }
      }
    }

    if (bitRead(player[guessY], byteToXY(guessX))) {
      cpuHits[guessY][guessX] = 1;
    } else {
      cpuMisses[guessY][guessX] = 1;
    }

    // win/lose condition
    int playerHitCount = 0;
    int cpuHitCount = 0;
    for (int i = 0; i < 7; i++) {
      cpuHitCount += cpuHits[i];
      playerHitCount += playerHits[i];
    }

    if (playerHitCount == cpuTotal) {
      blit(win);
      wait();
      return;
    } else if (cpuHitCount == playerTotal) {
      blit(lose);
      wait();
      return;
    }
  }
}

void canoyonRunner() {
  int playerPos = 4;
  int canyonPos = 2;
  int canyonWidth = 6;
  int rounds = 0;
  bool playerHasCrashed = false;
  byte display[8] = {
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
  };

  while (!playerHasCrashed) {
    blit(display);
    //TODO handle inputs

    if ((display[7] && playerPos) != 0) { //TODO make actually work right
      printDigits(rounds);
      wait();
      break;
    }

    rounds++; //TODO make work right
    if (rounds % 10 == 0) {
      canyonPos += random(-1, 1);
      canyonPos = clamp(canyonPos, 0, 7);
    } 
    if (rounds % 15 == 0) {
      canyonWidth -= 1;
      clamp(canyonWidth, 1, 8);
    }
    if (rounds % 20 == 0) {
      //TODO speed up player or something idk
    }
  }
}

void tetris() {
  //pieces[] = {{B11, B11, B11}, /* TODO */};
  /**lost = false;
  piece_loc = vector(0, 0)
  piece = random.choice(pieces);
  piecePlaced = false;
  board = {B0, B0, B0, B0, B0, B0, B0, B0};
  
  while (!lost) {
    display(board); // show all current placed pieces
    blit(piece, piece_loc); // display current active piece

    if (enoughTimeHasPassed || joystickDown()) { // player and time can move down. Piece does not move up, ever. 
      piece_loc[1] -= 1; // move the y down 1
    }

    if (buttonPressed()) {
      piece = rotate(piece);
    }

    //TODO make this so that sensitivity is less and you can actually be precise and stuff
    piece_loc[0] = clamp(piece_loc[0] + (1 * joystickXdir()), 0, 7); // handle x axis movement

    // maybe just: if ((piece && board) > 0) but no because needs to be at piece location whatever
    if ((piece[0] && board[piece_loc[1] - 1]) != 0) { // TODO fix this this is really wonky collision and doesn't handle edge case of colliding with floor
      board.blit(piece, piece_loc - 1); // also TODO fix
      piece_loc = 0, 0;
      piece = random.choice(pieces);
      score++;
    }

    flip(); // flip everything to the screen
  }
  displayNumbers(score);
  waitForButtonPress();
  break;*/

  /*
   * initialize and all that
   * pieces = {{B100, B110, B100}, etc.}
   * while not playerHasLost
   *    nextPiece = random.choice(pieces)
   *    pieceLoc = 0
   *    display(board)
   *    display(nextPiece)
   *    pieceLoc -= 1
   *    if joystick == left and pieceX > 0:
   *        pieceX -= 1
   *    elif joystick == right and pieceX < 8:
   *        pieceX += 1
   *    elif buttonPressed():
   *        rotate(nextPiece); // yeah rotation will be hard
   *    if joystick == down:
   *        //do some weird collision stuff
   *    if topRowHasAPiece:
   *        lose();
   *        quitToTitle();
   */
}

void unblockMe() {
  //yeah nope
}

void timer() {
  //useless? not even a game but sure I guess
}

void dinoGame() {
  bool playerHasCrashed = false;
  int score = 0;
  int player_vel = 0; // y-velocity
  int player_y = 0; // dist from ground
  int obs_x = 0;
  int obs_y = 0;
  int obstacle = 0;
  byte obstacles[5][2] = {
    {B00, B11},   // airplane
    {B010, B111}, // cloud
    {B001, B011}, // bush
    {B00, B11},   // rock
    {B01, B01}    // cactus
  };
  int speed = 1; // higher is faster
  while (!playerHasCrashed) {
    // do physics
    //TODO make this occur only once every X seconds or something
    player_vel += -2;
    player_y += player_vel;

    //TODO handle inputs
    //TODO add ground collision so you can't infinitely jump
    player_vel += 10;

    obs_x -= speed;
    if (obs_x > 5)  { // if the obstacle _could_ collide with the player
      //TODO implement collision
    }

    //TODO handle inputs

    //TODO make this occur only once the previous obstacle has left the screen
    obstacle = random(4);
    if (obstacle < 2) {
      obs_y = 3; //???
    } else {
      obs_y = 1; //???
    }
    //TODO draw obstacle

    matrix.clearDisplay(0); // clear the screen
    matrix.setRow(0, 0, B11111111); // draw the ground
    matrix.setLed(0, player_y + 1, 6, true); // draw the player

    //TODO make this occur only once every X seconds
    speed += 1;
  }
}

void snake() {
  // i literally have no idea
}

void reactionTimer() {
  // idk
}

void options() {
  // i have no idea how I'm going to implement this
  /*
   * mute/unmute
   * display brightness
   * AI difficulty?
   * multiplayer? (CPU vs 2nd person)
   * long button press threshold??
   * joystick sensitivity???
   * game speed???? (just have delay(gameSpeed); in all game loops or something that's kinda a trash way to do it but whatever)
   */
}

/*byte[8] pinball = {
  B00000000,
  B11000011,
  B10000001,
  B00011000,
  B00011010,
  B00000010,
  B10000110,
  B01001010
}*/

void pinball() {
  int lives = 3;
  int x = 7;
  int y = 0;
  int x_vel = 0;
  int y_vel = 0;
  while (lives > 0) {
    //TODO starting animation
    //TODO handle inputs
    //TODO actual physics
    //TODO win/lose conditions
    //TODO scoring
  }
}

void guessingGame() {
  /* Standard guessing game */
  int number = random(1, 20);
  int playerGuess = 0;
  bool playerHasGuessed;
  while (true) {
    playerHasGuessed = false;
    while (!playerHasGuessed) {
      printDigits(playerGuess);
      //TODO handle inputs
    }

    if (playerGuess == number) {
      blit(win);
      break;
    } else if (playerGuess < number) {
      //TODO display ^ up arrow ^ or something
    } else if (playerGuess > number) {
      //TODO display V down arrow V or something
    }
  }
}

void calculator() { // the heck daniel that's not even a game. Although, according to some random YouTube video title I saw you can speedrun it
  /* and here come the implementation comments
   * or not I think I can write it in (mostly) one go
   */
  int num1 = 0;
  int num2 = 0;
  double result = 0;
  int operation = 0;
  
  bool flag = false;
  while (!flag) {
    //TODO handle input
    printDigits(num1);
  }

  flag = false;
  while (!flag) {
    //TODO handle input
    printDigits(num2);
  }

  flag = false;
  while (!flag) {
    //TODO handle input
    printDigits(operation); //TODO add graphics, daniel. Not this number and chart crap
  }

  if (operation == 0) {
    result = num1 + num2;
  } else if (operation == 1) {
    result = num1 - num2;
  } else if (operation == 2) {
    result = num1 * num2;
  } else if (operation == 3) {
    result = num1 / num2;
  } else if (operation == 4) {
    result = pow(num1, num2);
  } else if (operation == 5) {
    result = num1 % num2;
  }

  printDigits(result);
}

void coinFlip() { // yeah this isn't a game so what deal with it
  byte heads[8] = {
    B00111100,
    B01000010,
    B10011001,
    B10100101,
    B10100101,
    B10011001,
    B01000010,
    B00111100,
  };
  byte tails[8] = {
    B00111100,
    B01000010,
    B10011001,
    B10011001,
    B10111101,
    B10111101,
    B01000010,
    B00111100,
  };
  int speed = 100;
  bool resultIsHeads = random(1);
  for (int i = 0; i < 20; i++) {
    if (resultIsHeads) {
      blit(heads);
    } else {
      blit(tails);
    }
    resultIsHeads = !resultIsHeads;

    delay(speed);
    if (i == 10) {
      speed = 500;
    } else if (i == 5) {
      speed = 800;
    }
  }
  wait();
}

void racingGame() { // yeah idk this is going to be weird and rather large (like, maybe larger than Battleship)
  int heading;
  int speed;
  int location;
  /**
   *       11111|11111   
          1     |     1  
         1      |      1 
        1       |       1
        1     11|11     1
        1    1  |  1    1
        1    1  |  1    1
        1    1  |  1    1
        =================
        1    1  |  1    1
        1    1  |  1    1
        1    1  |  1    1
        1     11|11     1
        1       |       1
          1     |      1 
           1    |     1  
            1111|11111  
   */
    /**
           11111|11111   
          1     |     1  
         1      |      1 
        1      1|11     1
        1     1 |  1    1
        1     1 | 1    1 
        1     1 |1    1  
        1     1 |1   1   
        =================
        1     1 |1   1   
        1     1 |1    1  
        1     1 | 1    1 
        1     1 |  1    1
        1      1|11     1
         1      |       1
          1     |     1  
           11111|11111   
   */
    /**
   *    11111111   
        1      1
        1      1
        1      1
        1      1
        1      1
        1      1
        1      1
        1      1
        ========
        1      1
        1  11  1
        1  11  1
        1      1
        1      1 
        111  111 
        1      1
        1      1
        ========
        1      1
        1      1
        11111  1
        1      1
        1      1
        1  11111
        1      1
        1      1
        ========
        1      1
        11111  1
        1      1
        1      1
        1  11111
        1      1
        1      1
   */
  /**
   *    11111111|11111111
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        =================
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        1       |       1
        11111111|11111111
   */
  byte locationOne[8] {
    B11111000,
    B00000100,
    B00000010,
    B00000001,
    B11000001,
    B00100001,
    B00100001,
    B00100001
  };
  byte locationTwo[8] {
    B00011111,
    B00100000,
    B01000000,
    B10000000,
    B10000011,
    B10000100,
    B10000100,
    B10000100
  };
  byte locationThree[8] = {
    B10000010,
    B10000010,
    B10000010,
    B10000011,
    B10000000,
    B01000000,
    B00100000,
    B00011111
  };
  byte locationFour[8] = {
    B00100001,
    B00100001,
    B00100001,
    B11000001,
    B00000001,
    B00000010,
    B00000100,
    B11111000
  };
}