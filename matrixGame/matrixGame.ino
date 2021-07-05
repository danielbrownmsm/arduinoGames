#include <LedControl.h>

// PINS
#define xPin A0
#define yPin A1
#define buttonPin A2 // !! NOTE !! b/c of the wiring and stuff, the analogRead will read some random values (seed PRNG, anyone?) when unpressed, and ~1020 when pressed (a check for >1010 should work just fine))
//TODO update wiring file and stuff

#define buzzerPin 8

#define dinPin 5
#define csPin 6
#define clkPin 7

// CONTROLS
//int x_val; // x value of the joystick
//int y_val; // y value of the joystick
//int buttonVal; // if the button is pressed or not

const unsigned int longPressThreshold = 1000; // threshold for a long button press
bool lastState; // previous state of the button (current minus 1)
unsigned long lastTime; // last time the button's state changed or something
unsigned long currTime; // current system time

// CONSTANT ARRAY SCREEN THINGS
// array to hold the state of the screen
byte screen[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
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

struct Battleship
{
  char x, y; // position
  char length; // length
  bool horizontal; // if it is horizontal (true) or vertical (false)
};

//TODO look into using char instead of int to save a byte of space everywhere if less than 127/255 for unsigned is needed

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

//TODO explain what this does
int byteToXY(int val) { // really I would say Cartesian but who has space to write that?
  return map(val, 0, 7, 7, 0);
}

void blit(byte array[8]) {
  for (int row = 0; row < sizeof(array); row++) {
    matrix.setRow(0, row, array[row]);
  }
}

//TODO documentation
void printDigits(int number) {
  int onesDigit = number % 10;
  int tensDigit = (number / 10) % 10; // because result is int it removes stuff after decimal
  for (int i = 0; i < 5; i++) { // b/c we only have 5 rows of number to display
    matrix.setRow(0, i+1, (digits[tensDigit][i] << 4) | digits[onesDigit][i]); // this _should_ work. Maybe. Honestly tho it prob won't
  }
}

void wait() {
  while (!digitalRead(buttonPin)) {
    delay(100);
  }
}

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

  //index = 0;
  //button.onPress(games[index].run());
  //CommandScheduler.run();
}

//TODO comment this code
void towerStackBlocks() {
  byte tower[8] = {B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110};
  int direction = 1;
  int score = 0;
  bool lastButtonState = false;
  bool buttonPress = false;

  while(true) {
    lastButtonState = buttonPress;
    buttonPress = digitalRead(buttonPin);
    
    blit(tower);
    if (bitRead(tower[0], 0) || bitRead(tower[0], 8)) { // ???
      direction *= -1;
    }

    tower[0] = ((direction > 0) ? (tower[0] >> 1) : (tower[0] << 1)); //???
    if (digitalRead(buttonPin) && lastButtonState == false) {
      tower[1] &= tower[0];

      // tower[6] is base
      // tower[0] is the moving level
      // only 7 items (0-6) b/c top one is always empty/blank/off
      for (int i = 6; i > 1; i--) { // move everything down a level
        tower[i] = tower[i-1];
      }
      tower[0] = tower[1];
      score++;
    }

    if (tower[0] == 0) {
      printDigits(score);
      wait();
      return; //???
    }

  }
}

void blackjack() {
  // set initial cards (p: 1, c: 1)
  // while not playerHasChosen:
  //    display(currentChoice)
  //    if stickMovesUp:
  //      currentChoice = hit
  //    elif stickMoveDown:
  //      currentChoice = stay
  //    if buttonPressed:
  //      playerHasChosen = true;
  //  
  //  if currentChoice is hit:
  //    playerCardTotal += random(1, 10)
  //  if computerCardTotal < 17:
  //    computerCardTotal += random(1, 10)
  //  if playerCardTotal > 21 || playerCardTotal < computerCardTotal < 22:
  //    display(lose);
  //  else:
  //    display(win);
  //  
  //  waitForQuitOrPlayAgainIDK();
  //  quitToTitle();
}

void battleship() {
  // set up field
  byte player[8] = { /* TODO */ };
  byte cpu[8] = { /* TODO */ };

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

  for (int shipNum = 0; shipNum < 4; shipNum++) {
    int shipSize = random(1, 4);
    int shipX = random(1, 8 - shipSize);
    // AGHH LOGIC
  }
  /*
   * while playerHasNotPlacedAllShips:
   *   display(placedShips)
   *   blink(currentShip)
   *   if buttonPressed:
   *      placeShip
   * 
   * placeCPUShips()
   * while not someoneHasWon:
   *    while not playerHasPickedTarget:
   *        slowBlink(cursor)
   *        fastBlink(playerHits)
   *        displaySolid(playerMisses)
   *        if buttonPressed:
   *          target = currentPos
   *    doPlayerHitOrMissLogic and updateBoardsSlashFields
   *    doCpuLogic()
   *    doWinOrLoseConditions()
   */
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

    if ((screen[7] && playerPos)  != 0) { //TODO make actually work right
      printDigits(rounds);
      wait();
      break;
    }

    rounds++;
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
  /*
   * initialize
   * while not PlayerHasCrashed:
   *  display(screenThingyOrSomething);
   *  if stick == left:
   *    playerPos -= 1
   *  elif stick == right:
   *    playerPos += 1
   *  doCollision()
   * 
   *  if enoughTimeHasPassed():
   *    moveCanyonLeftOrRightABit();
   *  elif moreThanEnoughTimeHasPassed():
   *    shrinkCanyon()
   *  elif wayMoreThanEnoughTimeHasPassedOrSomething maybe idk if you even want to include this:
   *    speedUpPlayerOrSomething()
   * 
   */
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
  /*playerHasCrashed = false;
  score = 0;
  player_vel = 0; // y-velocity
  player_y = 7; // dist from ground
  obstacle_pos = Vector(0, 0) // (x, y)
  obstacles = { /* TODO *//*};
  speed = 1000; // higher is slower
  clearBuffer();
  while (!playerHasCrashed) {
    drawLine(0, 7, 7, 7); // all draw calls operate on buffer, not actual screen
    obstacle = random.choice(obstacles);
    blit(obstacle, obstacle_pos); // blits onto buffer
    obstacle_pos[x] -= 1;

    // rest of stuff TODO

    fancyDelay(speed); // TODO make delay work but without delay so we don't miss inputs
    screen.flip()
  }*/
  /* 
   * initialize
   * while not playerHasCrashed:
   *    display(world)
   *    obstacle = random.choice(obstacles)
   *    display(obstacle)
   *    
   *    if player.collidesWith(obstalce):
   *      playerHasCrashed = true;
   *    
   *    if joystick == up and not player.isJumping:
   *      player.isJumping = true;
   *    elif joystick == down and not player.isDucking:
   *      player.isDucking = true;
   * 
   *    if player.isJumping:
   *        //TODO figure out player movement

   * quitToTitle();
   */
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
  /*
   * standard pinball. 3 lives. hitting things gives score. move left to up left paddle, right to up right paddle. Up to up both paddles.
   * 
   * 
   * 
   *  */
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
      //TODO display win or something
      break;
    } else if (playerGuess < number) {
      //TODO display ^ up arrow ^ or something
    } else if (playerGuess > number) {
      //TODO display V down arrow V or something
    }
  }
}