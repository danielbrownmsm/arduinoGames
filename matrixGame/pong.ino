class PongGame
{
private:
    // ball pos vars
    int ball_x;
    int ball_y;
    int x_vel;
    int y_vel;

    // limits
    int upper_bound;
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

    // difficulty???
    bool difficulty;

    // for sound are we doing a hardware kill switch or a software 'if sound is false don't play sound' or pinMode() shenanigans?
    // hardware would be more reliable, I say we go software though
    //  we can always delete it later

public:
    PongGame(/* args */);
    ~PongGame();
    bool Loop();
};

PongGame::PongGame(/* args */)
{
}

PongGame::~PongGame()
{
}

// return true if win, false if lose
bool PongGame::Loop() {
    // process input
    // if up and not at upper bound: move 1 up
    // elif down and not at lower bound: move 1 down
    // has a cooldown for movement so it can't move once every main loop iteration

    // ai input
    // if ball is up: move up
    // if ball is down: move down
    // but it has a cooldown for each movement so it can't move 1 every cycle

    // ball update
    // x += x_vel
    // y += y_vel
    // if upper bound > y > lower bound: y_vel *= -1
    
    // collision
    //TODO
    // could handle each edge case separately at the cost of speed and sanity but it's simpler
    // could also do some stuff to make it work with only a few statements at the cost of sanity
    // either way

    // win condition
    // if x < left bound: CPU_SCORE += 1
    // display happy screen followed by player score

    // elif x > right bound: player score += 1
    // display sad screen followed by CPU score
    
    // if CPU_SCORE > 10 (limit changes with difficulty, assuming we're actually implementing difficulty)
    //      return false;
    // elif player score > 10:
    //      return true;


    // x = 4
    // y = 4
    // x_vel = random()
    // y_vel = random()

    // update display
    // draw paddles()
    // draw ball()

    return false;
}