class BlockStack : public Game {
private:
    int direction;
    byte tower;

    int score;
    bool delayFlag;
    
public:
    BlockStack(/* args */);
    ~BlockStack();
    bool Loop();
};

BlockStack::BlockStack(/* args */)
{
}

BlockStack::~BlockStack()
{
}

bool BlockStack::Loop() {
    // process input
    //TODO

    // handle stacking
    //if (stack) {
    //}
    
    // update motion

    if (delayFlag) {
      if (bitRead(tower, byteToXY(7))) { // if we've hit the right side
            direction *= -1;
        } else if (bitRead(tower, byteToXY(0))) { // if we've hit the left side
            direction *= -1;
        }

    }
}

/**
 * while(1) {
      if (millis() - lastTime > waitTime) {
        lastTime = millis();
        if (direction == 1) {
          if (!bitRead(tower[0], byteToXY(0))) { // if we're not at the edge
            tower[0] = tower[0] << 1; // move it left
          } else { // else we're at the edge
            direction = 0;
          }
        } else {
          if (!bitRead(tower[0], byteToXY(7))) { // and then repeat everything
            tower[0] = tower[0] >> 1;
          } else {
            direction = 1;
          }
        }

        for (int i = 0; i < 7; i++) {
          matrix.setRow(0, i+1, tower[i]);
        }
      }


      if(!digitalRead(buttonPin)) { // if button press
        tower[1] &= tower[0]; // AND the moving level and top stationary level
        tower[0] = tower[1]; // actually update the top level
        
        if(tower[1] == B00000000) { // if there is nothing on the top level
          matrix.clearDisplay(0); // cear display 0

          int onesDigit = score % 10;
          int tensDigit = (score / 10) % 10; // because result is int it removes stuff after decimal
          for (int i = 0; i < 5; i++) { // b/c we only have 5 rows of number to display
            matrix.setRow(0, i+1, (digits[tensDigit][i] << 4) | digits[onesDigit][i]); // this _should_ work. Maybe. Honestly tho it prob won't
          }
          break;
        }
        score++;

        // tower[6] is base
        // tower[0] is the moving level
        // only 7 items (0-6) b/c top one is always empty/blank/off
        for (int i = 6; i > 1; i--) { // move everything down a level
          tower[i] = tower[i-1];
        }
        delay(50); // wait a bit because you can only unpress so fast
      }
      delay(50);
    }
    delay(1000); // wait so they can see the score
    byte inital[7] = {B00111111, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110};
    memcpy(tower, inital, sizeof tower);
    tower_y = 0;
    score = 0;
  }*/
