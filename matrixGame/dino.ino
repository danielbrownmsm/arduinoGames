// dino icon
// B00000110,
// B00001011,
// B10001111,
// B11011100,
// B11111110,
// B01111000,
// B01001000,
// B01001000,

class DinoGame
{
private:
    int score;
    bool jumping;

    int pos;
    int vel;
    int accel;
public:
    DinoGame(/* args */);
    ~DinoGame();
    bool Loop(double stick_x, double stick_y);
};

DinoGame::DinoGame(/* args */)
{
}

DinoGame::~DinoGame()
{
}

bool DinoGame::Loop(double stick_x, double stick_y) {
    // process input
    //TODO
    bool up = false;
    bool click = false;

    if (up || click) {
        if (on_ground) {
            jumping = true;
            accel = 20
        }
    }

    // jumping
    if (jumping) {
        //TODO
    }

    if (speedFlag) {
        vel += accel
        pos += vel

        if (pos < 0) {
            pos = 0
            vel = 0
            accel = 0
        }
    }

    return false;
}