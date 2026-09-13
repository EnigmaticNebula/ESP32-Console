class Game {
    public:
    bool loaded = false;
    int game_id; 
    
    virtual void joystick_up();
    virtual void joystick_right();
    virtual void joystick_left();
    virtual void joystick_down();
    virtual void joystick_action();
    virtual void button1();
    virtual void button2();
    virtual void button3();
    virtual void rotary_encoder_clockwise();
    virtual void rotary_encoder_anticlockwise();
    virtual void potentiometer_change(int new_value);
    virtual void iterate();
    virtual void load();
    virtual void unload();
};