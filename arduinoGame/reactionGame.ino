class ReactionGame : public Game {
public:
    byte icon[8] {
        B01000010,
        B10011001,
        B00100100,
        B01000010,
        B10011001,
        B00100100,
        B01000010,
        B10011001
    };
    byte screen[8] = {
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000
    };
    byte right_arrow[8] = {
        B11111000,
        B01111100,
        B00111110,
        B00011111,
        B00011111,
        B00111110,
        B01111100,
        B11111000,
    };
    byte down_arrow[8] = {
        B10000001,
        B11000011,
        B11100111,
        B11111111,
        B01111111,
        B01111110,
        B00111100,
        B00011000,
    };
    byte left_arrow[8] = {
        B00001111,
        B00111110,
        B01111100,
        B11111000,
        B11111000,
        B01111100,
        B00111110,
        B00011111,
    };
    byte up_arrow[8] = {
        B00011000,
        B00111100,
        B01111110,
        B11111111,
        B11111111,
        B11100111,
        B11000011,
        B10000001,
    };
    byte center_circle[8] = {
        B00000000,
        B00011000,
        B00111100,
        B01111110,
        B01111110,
        B00111100,
        B00011000,
        B00000000,
    };



    int score = 0;
    unsigned long time = 0.0;
    int type = 0; // 1 is left, 2 right, 3 up, 4 down, 5 button press, 6 all but button press, 7 all
    virtual void update(double stick_x, double stick_y, boolean stick_b, byte* prev_screen, long time, int speed, boolean mute) {

    };
};


