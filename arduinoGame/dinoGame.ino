class DinoGame: public Game {
    public:
        byte icon[8] {
            B00000000,
            B00000000,
            B00000000,
            B00000010,
            B01001010,
            B01000100,
            B11111111,
            B00000000
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
        boolean jumping = false;
        int y_loc = 0;
        int cacti_loc = 8;
        int bird_loc = 8;
        virtual void update(double stick_x, double stick_y, boolean stick_b, byte* prev_screen, long time, int speed, boolean mute) {
            
        };
};
