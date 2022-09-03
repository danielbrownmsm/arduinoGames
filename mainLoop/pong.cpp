class PongGame : public Game
{
private:
    // ball pos vars
    int ball_x = 4;
    int ball_y = 4;
    int x_vel = 0;
    int y_vel = 0;

    // limits
    int upper_bound; //TODO
    int lower_bound;
    int left_bound;
    int right_bound;

    // paddles (we're doing a paddle size of 2 right? 3 would fill too much of the screen, and 1 isn't a paddle)
    int player_x;
    int cpu_x;

    // score
    int player_score;
    int cpu_score;
    int win_score;

    // game update/tick flags
    bool player_moved;
    bool cpu_moved;
    bool ball_moved;

    bool reset_ball;

    // difficulty???
    bool difficulty;

    // for sound are we doing a hardware kill switch or a software 'if sound is false don't play sound' or pinMode() shenanigans?
    // hardware would be more reliable, I say we go software though
    //  we can always delete it later

public:
    LedControl m_screen;
    PongGame(LedControl screen);
    ~PongGame();
    bool Loop(double stick_x, double stick_y);
};

PongGame::PongGame(LedControl screen) {
  //m_screen = screen;
}

PongGame::~PongGame()
{
}

// return true if win, false if lose
bool PongGame::Loop(double stick_x, double stick_y) {
    // check and update 'cooldown' flags
    //XXX actually do we even need the flags though? if we just mark down the last time something moved then if the time interval between that and now
    //    is large enough then we're good to go because we're going to be calculating that interval every loop anyways unless nobody moves at all
    //    but who is going to not move at all? the cycles we need are the ones where somebody moves anyways so we ought to get rid of flags
    //    or make them into can_player_move because right now the FLAGS AREN'T EVEN DOING ANYTHING DANIEL YOU DUMB PIECE OF CRAP
    if (player_moved && enoughTimeHasPassed) {
        player_moved = false;
    }

    if (cpu_moved && enoughTimeHasPassed) {
        cpu_moved = false;
    }

    if (ball_moved && enoughTimeHasPassed) {
        ball_moved = false;
    }


    // process input
    // if up and not at upper bound: move 1 up
    if (up && player_x < upper_bound) { // need to sub 1???
        player_x += 1;
        player_moved = true;

    // elif down and not at lower bound: move 1 down
    } else if (down && player_x > lower_bound) {
        player_x -= 1;
        player_moved = true;
    }

    // ai input
    // if ball is up: move up
    if (ball_x > cpu_x) {
        if (cpu_x < upper_bound) {
            cpu_x += 1;
            cpu_moved = true;
        }

    // if ball is down: move down
    } else if (ball_x < cpu_x) {
        if (cpu_x < lower_bound) {
            cpu_x -= 1;
            cpu_moved = true;
        }
    }

    // ball update
    ball_x += x_vel;
    ball_y += y_vel;

    //FIXME do we want to do boundary checking before or after position update
    //      probably want to do it after. . . need to subtract 1 from upper and/or lower bounds
    //      so changes take effect while the ball is still on the screen?
    // if upper bound > y > lower bound: y_vel *= -1
    if (ball_y > upper_bound || ball_y < lower_bound) {
        ball_y *= -1;
    }
    
    // collision
    //TODO
    // could handle each edge case separately at the cost of speed and sanity but it's simpler
    // could also do some stuff to make it work with only a few statements at the cost of sanity
    // either way

    // win condition
    // if x < left bound: CPU_SCORE += 1
    if (ball_x < left_bound) {
        cpu_score += 1;
        // display sad screen followed by CPU score
        //TODO make sad screen function
        // not sure how we'd access that from here but
        // oh shoot we can't access any of the draw functions either. . .
        // unless we make a separate DrawUtil or GameUtil thing that we import. . .

        reset_ball = true;

    // elif x > right bound: player score += 1
    } else if (ball_x > right_bound) {
        player_score += 1;
        // display happy screen followed by player score

        reset_ball = true;

    }

    // if CPU_SCORE > 10 (limit changes with difficulty, assuming we're actually implementing difficulty)
    if (cpu_score > 10) {
        return false;

    // elif player score > 10:
    } else if (player_score) {
        return true;
    }

    if (reset_ball) {
        ball_x = 4; // I think 4 is good idk
        ball_y = 4; 

        x_vel = random();
        y_vel = random();
    }

    // update display
    // draw paddles()
    // draw ball()

    return false;
}
