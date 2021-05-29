class Game {
    public:
        byte icon[8];
        byte screen[8];
        virtual void update(double stick_x, double stick_y, bool stick_b, byte* prev_screen, long time, int speed, bool mute) = 0;
        virtual byte* get_screen() = 0;
};

void setup() {
    
}
void loop() {
    
}
