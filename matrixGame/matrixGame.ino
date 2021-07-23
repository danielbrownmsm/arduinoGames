#include <LedControl.h>

// PINS
#define xPin A0
#define yPin A1
#define buttonPin 3 //TODO update wiring/circuit file or whatever
// !! NOTE !! the button reads 0 for pressed and 1 for unpressed. Read it accordingly

#define buzzerPin 8

#define dinPin 5
#define csPin 6
#define clkPin 7

#define WAIT_MACRO while(digitalRead(buttonPin)) {delay(100);}

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
  {B00000010, B00000101, B00000101, B00000101, B00000010},
  {B00000001, B00000011, B00000001, B00000001, B00000001}, 
  {B00000110, B00000001, B00000011, B00000100, B00000111}, 
  {B00000110, B00000001, B00000110, B00000001, B00000110}, 
  {B00000101, B00000101, B00000111, B00000001, B00000001}, 
  {B00000111, B00000100, B00000110, B00000001, B00000110}, 
  {B00000011, B00000100, B00000110, B00000101, B00000011}, 
  {B00000111, B00000001, B00000010, B00000010, B00000100}, 
  {B00000111, B00000101, B00000111, B00000101, B00000111}, 
  {B00000110, B00000101, B00000111, B00000001, B00000110}
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
  pinMode(buttonPin, INPUT_PULLUP);
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
  for (unsigned int row = 0; row < 8; row++) {
    matrix.setRow(0, row, array[row]);
  }
}

// prints a number (between 0 and 99) to the screen. This is non-blocking
void printDigits(int number) {
  matrix.clearDisplay(0);
  int onesDigit = number % 10;
  int tensDigit = (number / 10) % 10; // because result is int it removes stuff after decimal
  for (int i = 0; i < 5; i++) { // b/c we only have 5 rows of number to display
    matrix.setRow(0, i+2, (digits[tensDigit][i] << 4) | digits[onesDigit][i]); // yes I just copy-pasted from old code. deal with it
  }
  delay(50);
}

void dispTwoArrays(byte arrayOne[8], byte arrayTwo[8]) { // literally only a helper func for battleship
  for (int i = 0; i < 8; i++) {
    matrix.setRow(0, i, arrayOne[i] | arrayTwo[i]);
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
  //Serial.print("starting...");
  //delay(300);
  /*Serial.print("X: ");
  Serial.print(analogRead(xPin));
  Serial.print(" | Y: ");
  Serial.print(analogRead(yPin));
  Serial.print(" | SW: ");
  Serial.print(digitalRead(buttonPin));
  Serial.print("\n");
  delay(300);*/

  Serial.println("tower");
  matrix.clearDisplay(0);
  towerStackBlocks();
  
  Serial.println("21");
  matrix.clearDisplay(0);
  blackjack();
  
  Serial.println("canyon");
  matrix.clearDisplay(0);
  canyonRunner();
  
  Serial.println("guess");
  matrix.clearDisplay(0);
  guessingGame();
  
  Serial.println("coin");
  matrix.clearDisplay(0);
  coinFlip();
  
  /*Serial.println("ship");
  matrix.clearDisplay(0);
  battleship();
  
  Serial.println("dino");
  matrix.clearDisplay(0);
  dinoGame();*/
  
  //Serial.println("tron");
  //matrix.clearDisplay(0);
  //tron();
  
  /*Serial.println("bowl");
  matrix.clearDisplay(0);
  bowling();
  
  Serial.println("calc");
  matrix.clearDisplay(0);
  calculator();*/
  
  //index = 0;
  //button.onPress(games[index].run());
  //CommandScheduler.run();
}

// DONE and tested still needs work
void towerStackBlocks() {
  byte tower[8] = {B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110};
  int dir = 1; // 1 for moving right, -1 for moving left
  int score = 0;
  bool lastButtonState = false; // used for
  bool buttonPress = false; // buffering/toggling inputs
  bool stack = false;
  unsigned long now = 0;
  unsigned long lastTime = 0;
  unsigned int movSpeed = 300;
  //TODO add difficulty so it gets faster the higher you go, regardless of tower width

  while(true) {
    lastButtonState = buttonPress;
    buttonPress = !digitalRead(buttonPin);

    if (buttonPress && !lastButtonState) { // if the button is pressed
        stack = true; // then we should stack
        //Serial.println("BP");
    }
    
    blit(tower);
    now = millis();

    if (now - lastTime > movSpeed) {
        lastTime = now;
        if (bitRead(tower[0], 0) || bitRead(tower[0], 7)) { // if we've reached one of the edges
          dir *= -1; // invert the direction
        }
    
        if (stack) { // if we need to stack
          stack = false;
          tower[1] &= tower[0]; // shave off the bits that missed
    
          // tower[6] is base
          // tower[0] is the moving level
          // only 7 items (0-6) b/c top one is always empty/blank/off
          for (int i = 7; i > 1; i--) { // move everything down a level
            tower[i] = tower[i-1];
          }
          tower[0] = tower[1]; // and stack it
          //TODO fix it stacking two at a time and all that
          score++;
        }

        tower[0] = ((dir > 0) ? (tower[0] >> 1) : (tower[0] << 1)); // shift the active level left or right, depending on direction
    
        if (tower[0] == 0) { // lose condition
          //Serial.println("lost");
          printDigits(score);
          WAIT_MACRO
          return; // exit or something idk
        }
    }
    delay(10);

  }
}

// DONE and tested and works. digits might need some work but it's just graphics so /shrug
void blackjack() {
  //graphics
  byte hit[8] = {
    B00000000,
    B00011000,
    B00011000,
    B01111110,
    B01111110,
    B00011000,
    B00011000,
    B00000000,
  };
  byte stay[8] = {
    B00000000,
    B00000000,
    B00000000,
    B01111110,
    B01111110,
    B00000000,
    B00000000,
    B00000000,
  };
  // handle CPU turn
  int cpuTotal = random(1, 10);
  while (cpuTotal < 17) {
    cpuTotal += random(1, 10);    
  }
  //Serial.println(cpuTotal);

  // handle player turn
  int playerTotal = random(1, 10);
  printDigits(playerTotal);
  delay(200);
  WAIT_MACRO
  delay(300);

  bool playerStays = false;
  bool playerIsChoosingHit = false;
  bool buttonState = false;
  bool lastButtonState = false;
  int lastJoyState = 0;
  int joyState = 0;
  while (!playerStays) {
    lastButtonState = buttonState;
    buttonState = !digitalRead(buttonPin);

    joyState = analogRead(yPin);
    if (joyState > 800) {
      joyState = 1;
    } else if (joyState < 200) {
      joyState = -1;
    } else {
      joyState = 0;
    }

    if (joyState == 1 && lastJoyState != 1) {
      playerIsChoosingHit = true;
    } else if (joyState == -1 && lastJoyState != -1) {
      playerIsChoosingHit = false;
    }

    if (playerIsChoosingHit) {
        blit(hit);
    } else {
        blit(stay);
    }

    if (buttonState && !lastButtonState) {
      if (playerIsChoosingHit) {
        playerTotal += random(1, 10);
        printDigits(playerTotal);
        delay(200);
        WAIT_MACRO
        if (playerTotal > 21) {
          playerStays = true;
        }
      } else {
        playerStays = true;
      }
    }
    delay(20);
  }

  if (cpuTotal > 21 || (playerTotal > cpuTotal && playerTotal < 22)) {
    blit(win);
  } else {
    blit(lose);
  }
  delay(300);
  WAIT_MACRO
}

// getting there
void battleship() { // this is so freakin long
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
  int lastXState = 0;
  int lastYState = 0;
  int xState = 0;
  int yState = 0;
  bool buttonState = false;
  bool lastButtonState = false;

  while(!playerDonePlacing) {
    blit(player);

    lastButtonState = buttonState;
    lastXState = xState;
    lastYState = yState;

    xState = analogRead(xPin);
    if (xState > 800) {
      xState = 1;
    } else if (xState < 200) {
      xState = -1;
    } else {
      xState = 0;
    }

    yState = analogRead(xPin);
    if (yState > 800) {
      yState = 1;
    } else if (yState < 200) {
      yState = -1;
    } else {
      yState = 0;
    }

    buttonState = !digitalRead(buttonPin);

    // now for, you know, actual ships
    bool horizontal = random(1);
    int shipSize = random(1, 4);
    if (horizontal) {
      int shipX = random(7 - shipSize);
      int shipY = random(7);
    } else {
      int shipX = random(7);
      int shipY = random(7 - shipSize);
    }

    //TODO handle placing ships
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
      bitSet(playerHits[guessY], guessX);
    } else {
      bitSet(playerMisses[guessY], guessX);
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
      bitSet(cpuHits[guessY], guessX);
    } else {
      bitSet(cpuMisses[guessY], guessX);
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
      WAIT_MACRO
      return;
    } else if (cpuHitCount == playerTotal) {
      blit(lose);
      WAIT_MACRO
      return;
    }
  }
}

/**
 *  bool buttonState = false;
  bool lastButtonState = false;
  int lastJoyState = 0;
  int joyState = 0;
  while (!playerStays) {
    lastButtonState = buttonState;
    buttonState = digitalRead(buttonPin);

    joyState = analogRead(yPin);
    if (joyState > 800) {
      joyState = 1;
    } else if (joyState < 200) {
      joyState = -1;
    } else {
      joyState = 0;
    }
 */

// DONE (i think)
void canyonRunner() {
  int playerPos = 4;
  int canyonPos = 2;
  int canyonWidth = 6;
  int rounds = 0;
  int score = 0;
  bool playerHasCrashed = false;
  byte disp[8] = {
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
  };
  int previousPoses[8] {
    0, 0, 0, 0, 0, 0, 0, 0
  }; //HACK TODO FIXME BUG XXX AND SO ON basically i just store the last 8 positions so I know which canyonPos to check against for out-of-bounds because you can escape if you
  // move the stick at the right time so there needs to be OOB but you can't use the current canyonPos because that is seven rows above you and you might not be able to move
  // to get in it with the walls where they are in _your_ level so if there is a better way to do this then sure but the (time) complexity of checking to determine bounds 
  // from just the display array seems too high but if you want to try it go ahead
  int previousWidths[8] = {0,0,0,0,0,0,0,0};
  int lastJoyState = 0;  
  int joyState = 0;
  unsigned int playerSpeed = 400;
  unsigned long lastTime = 0;
  unsigned long now = 0;
  bool updatePlayerPos = false;
  bool movePlayerRight = false;
  int roundsToWait = random(3, 5);
  bool blinkState = true;

  while (!playerHasCrashed) {
    now = millis();
    lastJoyState = joyState;
    blinkState = !blinkState;
    matrix.setLed(0, 7, byteToXY(playerPos), blinkState);

    joyState = analogRead(xPin);
    if (joyState > 800) {
      joyState = 1;
    } else if (joyState < 200) {
      joyState = -1;
    } else {
      joyState = 0;
    }

    if (joyState == 1 && lastJoyState != 1) {
      updatePlayerPos = true;
      movePlayerRight = true;
    } else if (joyState == -1 && lastJoyState != -1) {
      updatePlayerPos = true;
      movePlayerRight = false;
    }
    Serial.println(canyonPos);
    if ((disp[7] & (1 << playerPos)) && rounds > 3) {
        printDigits(score);
        WAIT_MACRO
        return;
    } else if (playerPos >= previousPoses[7] || playerPos <= previousPoses[7] - previousWidths[7]) {
        if (rounds > 7) {
            Serial.print(playerPos);
            Serial.print("|");
            Serial.print(previousPoses[7]);
            Serial.print("|");
            Serial.print(previousWidths[7]);
            Serial.print("\n");
            //printDigits(score);
            //WAIT_MACRO
            //return;            
        }
    }

    if (now - lastTime > playerSpeed) { // if time has passed
      for (int i = 7; i > 0; i--) { // move everything down a level
        disp[i] = disp[i-1];
        //Serial.println(i);
      }
      disp[0] = 0;
      bitSet(disp[0], canyonPos); // can I get a yeah for doing exactly what we did previously?
      bitSet(disp[0], canyonPos + canyonWidth); // and for storing data on the screen?
      blit(disp);
 
      lastTime = now; // update time
      rounds++; // increase rounds

      if (rounds % 30 == 0) { // every 15 rounds
        canyonWidth -= 1; // shrink the canyon
        canyonWidth = clamp(canyonWidth, 3, 8); // but make sure it doesn't get too small
      }
      
      if (rounds % roundsToWait == 0) { // every 3 rounds
        roundsToWait = random(3, 5);
        int num = random(-1, 1);
        if (num == 0) {
            num = 1;
        }
        canyonPos += num; // move the canyon to the left or right
        
        if (canyonPos == 7) {
            canyonPos -= 1;
        } else if (canyonPos == 0) {
            canyonPos += 1;
        }

        previousPoses[0] = canyonPos;//TODO optimize this
        previousWidths[0] = canyonWidth;
        for (int i = 7; i > 0; i--) {
            previousPoses[i] = previousPoses[i - 1];
            previousWidths[i] = previousWidths[i - 1];
        }
        
        score++;
      }

      if (rounds % 10 == 0) { // every 20 rounds
        playerSpeed -= 1; // speed player/ticks up
        playerSpeed = clamp(playerSpeed, 100, 3000);
      }

      if (updatePlayerPos) { // update player position
        if (movePlayerRight && playerPos < 7) {
          playerPos += 1;
        } else {
          if (playerPos > 0) {
              playerPos -= 1;
          }
        }
        movePlayerRight = false;
        updatePlayerPos = false;
      }
    }
    delay(50);
  }
}

// close
void tetris() {
  byte pieces[3][3] = {
    {
      B1,
      B1,
      B1,
    },
    {
      B00,
      B01,
      B11
    },
    {
      B00,
      B11,
      B11
    },
  };

  bool lost = false;
  int pieceX = 7;
  int pieceY = 7;
  int piece = random(0, 2);
  bool piecePlaced = false;
  byte board[8] = {B0, B0, B0, B0, B0, B0, B0, B0};
  bool enoughTimeHasPassed = false;
  int score = 0;
  int rotation = 0;
  bool lastButtonState = false;
  bool buttonState = false;
  
  while (!lost) {
    lastButtonState = buttonState;
    buttonState = !digitalRead(buttonPin);

    blit(board); // show all current placed pieces
    //TODO blit(piece, piece_loc); // display current active piece

    if (enoughTimeHasPassed /* || joystickDown()*/) { // player and time can move down. Piece does not move up, ever. 
      pieceY -= 1; // move the y down 1
    }

    if (buttonState && !lastButtonState) {
      rotation = (++rotation) % 4;
      lastButtonState = true;
    }

    //TODO handle X movement
    pieceX = clamp(pieceX + (1), 0, 7);

    if (pieceY && bitRead(board[pieceY + 1], byteToXY(pieceX))) {
      //TODO add piece to board
      score++;
      piece = random(0, 2);
      pieceX, pieceY = 7;
    }

    //TODO handle level clears

    if (board[0] > 0) {
      lost = true;
    }
  }
  printDigits(score);
  WAIT_MACRO
}

// not even close
void unblockMe() {
  //yeah nope
}

// not even close
void timer() {
  //useless? not even a game but sure I guess
}

// almost DONE
void dinoGame() {
  bool playerHasCrashed = false;
  int score = 0;
  int player_vel = 0; // y-velocity
  int player_y = 0; // dist from ground
  int obs_x = 0;
  int obs_y = 0;
  int obstacle = 0;
  byte obstacles[5][2] = {
    {B000, B011}, // airplane
    {B010, B111}, // cloud
    {B001, B011}, // bush
    {B000, B011}, // rock
    {B001, B001}  // cactus
  };
  int speed = 1; // higher is faster
  bool isJumping = false;
  bool lastButtonState = false;
  bool buttonState = false;
  while (!playerHasCrashed) {
    lastButtonState = buttonState;
    buttonState = !digitalRead(buttonPin);

    // do physics
    //TODO make this occur only once every X seconds or something
    player_vel += -2;
    player_y += player_vel;

    if (player_y == 0) { // reset jump flag
      isJumping = false;
    }

    // handle jumping and inputs
    if (!isJumping && buttonState && !lastButtonState) { // can't infinitely jump
      player_vel += 5;
      isJumping = true;
    }

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

    matrix.clearDisplay(0); // clear the screen
    matrix.setRow(0, 0, B11111111); // draw the ground
    matrix.setLed(0, player_y + 1, 6, true); // draw the player


    //TODO make this occur only once every X seconds
    speed += 1;
  }
}

// not even close
void snake() {
  // i literally have no idea
}

// not even close
void reactionTimer() {
  // idk
}

// not even close
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

// not close
void pinball() {
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

// DONE and tested
void guessingGame() {
  /* Standard guessing game */
  int number = random(1, 20);
  int playerGuess = 0;
  bool playerHasGuessed = false;
  int joyState = 0;
  int lastJoyState = 0;
  bool lastButtonState = false;
  bool buttonState = false;
  byte upArrow[8] = {
    B00000000,
    B00011000,
    B00111100,
    B01011010,
    B00011000,
    B00011000,
    B00011000,
    B00000000,
  };
  byte downArrow[8] = {
    B00000000,
    B00011000,
    B00011000,
    B00011000,
    B01011010,
    B00111100,
    B00011000,
    B00000000,
  };

  while (true) {
    playerHasGuessed = false;
    while (!playerHasGuessed) {
      lastJoyState = joyState;
      lastButtonState = buttonState;
      printDigits(playerGuess);

      // handle inputs
      joyState = analogRead(yPin);
      if (joyState > 800) {
        joyState = 1;
      } else if (joyState < 200) {
        joyState = -1;
      } else {
        joyState = 0;
      }

      // handle inc/dec
      if (joyState == 1 && lastJoyState != 1) {
        playerGuess += 1;
      } else if (joyState == -1 && lastJoyState != -1) {
        playerGuess -= 1;
      }

      // wrap inputs
      if (playerGuess < 1) {
        playerGuess = 20;
      } else if (playerGuess > 20) {
        playerGuess = 1;
      }

      // handle guess
      buttonState = !digitalRead(buttonPin);
      if (buttonState && !lastButtonState) {
        playerHasGuessed = true;
      }
      delay(50);
    }

    if (playerGuess == number) {
      blit(win);
      delay(100);
      WAIT_MACRO
      break;
    } else if (playerGuess < number) {
      blit(upArrow);
      delay(100);
      WAIT_MACRO
    } else if (playerGuess > number) {
      blit(downArrow);
      delay(100);
      WAIT_MACRO
    }
  }
}

// getting there
void calculator() { // the heck daniel that's not even a game. Although, according to some random YouTube video title I saw you can speedrun it
  int num1 = 0;
  int num2 = 0;
  double result = 0;
  int operation = 0;
  int joyState = 0;
  int lastJoyState = 0;
  bool buttonState = false;
  bool lastButtonState = false;
  
  
  bool flag = false;
  while (!flag) {
      lastJoyState = joyState;
      lastButtonState = buttonState;
      printDigits(num1);

      // handle inputs
      joyState = analogRead(yPin);
      if (joyState > 800) {
        joyState = 1;
      } else if (joyState < 200) {
        joyState = -1;
      } else {
        joyState = 0;
      }

      // handle inc/dec
      if (joyState == 1 && lastJoyState != 1) {
        num1 += 1;
      } else if (joyState == -1 && lastJoyState != -1) {
        num1 -= 1;
      }

      // wrap inputs
      if (num1 < 0) {
        num1 = 99;
      } else if (num1 > 99) {
        num1 = 0;
      }

      // handle guess
      buttonState = !digitalRead(buttonPin);
      if (buttonState && !lastButtonState) {
        flag = true;
      }
      delay(50);
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

// DONE and tested
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
  bool resultIsHeads;
  if (random(10) % 2 == 0) {
    resultIsHeads = false;
  } else {
    resultIsHeads = true;
  }

  for (int i = 0; i < 20; i++) {
    if (resultIsHeads) {
      blit(heads);
    } else {
      blit(tails);
    }
    resultIsHeads = !resultIsHeads;

    delay(speed);
    if (i == 15) {
      speed = 500;
    } else if (i == 10) {
      speed = 300;
    }
  }
  WAIT_MACRO
}

// not close
void racingGame() { // yeah idk this is going to be weird and rather large (like, ~maybe~ def larger than Battleship)
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

// almost DONE
void tron() {
  byte field[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int playerX = 0;
  int pXvel = 1;
  int pYvel = 0;
  int playerY = 4;

  int cpuX = 7; // start opposite player
  int cXvel = -1;
  int cYvel = 0;
  int cpuY = 4;

  int xState = 0;
  int lastXState = 0;
  int yState = 0;
  int lastYState = 0;

  unsigned long now = 0;
  unsigned long lastTime = 0;
  bool blinkState = false;

  bool over = false;
  while (!over) {
    now = millis();
    lastXState = xState;
    lastYState = yState;
    
    xState = analogRead(xPin);
    yState = analogRead(yPin);

    if (xState > 800) {
      xState = 1;
    } else if (xState < 200) {
      xState = -1;
    } else {
      xState = 0;
    }

    if (yState > 800) {
      yState = 1;
    } else if (yState < 200) {
      yState = -1;
    } else {
      yState = 0;
    }

    if (now - lastTime > 400) {
      lastTime = now;
      pXvel = clamp(xState, -1, 1);
      pYvel = clamp(yState, -1, 1);

      playerX += pXvel;
      playerY += pYvel;

      for (int i = 0; i < 8; i++) {
        //TODO handle AI
      }
      
      cpuX += cXvel;
      cpuY += cYvel;

      blit(field);
      //if (bitRead(field[playerY], byteToXY(playerX))) { // player collision
      //  over = true;
      //  blit(lose);
      //  delay(100);
      //  WAIT_MACRO
      //} else if (bitRead(field[cpuY], cpuX)) { // computer collision
      //  over = true;
      //  blit(win);
      //  delay(100);
      //  WAIT_MACRO
      //}
      //bitSet(field[playerY], playerX);
      //bitSet(field[cpuY], cpuX);
      matrix.setLed(0, playerY, playerX, 1);
    }


  }
}

// not even close
void ticTacToe() {
  // i don't care enough for this one
}

// getting there
void bowling() {
  bool hasBowled = false;
  int score = 0;
  int angle = 0;
  int xPosition = 4;
  byte pins[4] = {
    B10101010,    
    B01010100,
    B00101000,
    B00010000,
  };

  while (!hasBowled) {
    angle = analogRead(xPin) - 510;
    if (analogRead(yPin) > 800) {
      hasBowled = true;
    }
  }

  for (int i = 0; i < 7; i++) {
    // b/c xPosition is int, automatically ~rounds~ chops off small bits
    xPosition += tan(angle) / 10; // 3 is speed. tune until graphics look good.
    /**
     *      X
     *  ___________
     *  \          |
     *   \         |
     *    \        | speed
     *     \       |
     *      \      |
     *       \angle|
     *        \    |
     *         \   |
     *          \  |
     *           \ |
     *            \|
     * tan(angle) = X / speed
     */

    matrix.clearDisplay(0);
    matrix.setRow(0, i, xPosition);

    delay(200);
  }

  if (xPosition > 7 || xPosition < 0) {
    blit(lose);
    WAIT_MACRO
    //TODO do something idk
  }

  //TODO make ball bigger
  //TODO handle animation
  //TODO handle physics
  //round();

}
