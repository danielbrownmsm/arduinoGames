/*int board[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}};
byte matrix[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
bool playerTurn = false;

int

void setup {
  
}

void loop {
  
}

void loop() {
  blinkState = !blinkState;*/
  /*
   * 8x8 matrix of LEDs
   * 3x3 tic tac toe board
   * if stick_x is rigt:
   *  if cursor is not too far rigt:
   *    move cursor right
   * if stick_x is left:
   *  if cursor is not too far left:
   *    move cursor left
   * if stick_y is up:
   *  if cursor is not too far up:
   *    move cursor up
   * if stick_y is down:
   *  if cursor is not too far down:
   *    move cursor down
   * if buttonClicked:
   *  if cell is not already occupied:
   *    set cell to 1
   *    set playerTurn to FALSE
   *    
   * playerTurn boolean
   * if it is not playerTurn:
   *  sum the rows
   *  if the sum of a row is == 2:
   *    find the zero in the row and set it to -1
   *    break
   *  elif the sum of a column is 2:
   *    find the zero in the column and set it to -1
   *    break
   *  else:
   *    select a random cell
   *    if the cell is not already filled:
   *      set it to -1
   *      set playerTurn to TRUE
   *      break
   *    otherwise, keep looking
   * 
   * //draw the board
   * scale 0,1,2 to 0,1,2,3,4,5,6,7
   * draw the grid
   * for each cell:
   *  draw it at the scaled coordinates
   *  if the cell is a player cell (i.e. == 1 and != -1):
   *    fill out the square
   */
  /*
  for (int i = 0; i < 8; i++) {
    world[i] |= B00100100;
    matrix.setRow(0, i, world[i]);
  }
  matrix.setRow(0, 2, B11111111);
  matrix.setRow(0, 5, B11111111);
  
  x_val = analogRead(xPin);
  y_val = analogRead(yPin);

  if (x_val > 700) {
    if (x < 7) {
      x += 3;      
    }
  }
  else if (x_val < 300) {
    if (x > 2) {
      x -= 3;
    }
  }

  if (y_val > 700){
    if (y < 7) {
      y += 3;
    }
  }
  else if (y_val < 300) {
    if (y > 2) {
      y -= 3;
    }
  }

  bitClear(world[prev_x], prev_y);
  bitClear(world[prev_x-1], prev_y);
  bitClear(world[prev_x], prev_y-1);
  bitClear(world[prev_x-1], prev_y-1);

  if (blinkState) {
    bitSet(world[x], y);
    bitSet(world[x-1], y);
    bitSet(world[x], y-1);
    bitSet(world[x-1], y-1);
  }
  else {
    bitClear(world[x], y);
    bitClear(world[x-1], y);
    bitClear(world[x], y-1);
    bitClear(world[x-1], y-1);
  }

    
  prev_x = x;
  prev_y = y;
  
  //tone(buzzerPin, 1000, 100);
  delay(200);
*/
