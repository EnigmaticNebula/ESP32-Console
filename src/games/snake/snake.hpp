#include <Arduino.h>
#include <games\game.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
using namespace std;

class Snake : public Game {
    private:
    enum class Direction {
        up,
        down,
        left,
        right
    };

    struct Vector2D {
        int dx;
        int dy;
    };

    struct SnakeElementPosition {
        int col;
        int row;
    };

    Vector2D get_direction_vector(Direction direction) {
        switch (direction) {
            case Direction::up: return {0, 1};
            case Direction::down: return {0, -1};
            case Direction::left: return {-1, 0};
            case Direction::right: return {1, 0};
        }
        return {0, 0}; 
    }
    unsigned int iteration_delay = 200;
    unsigned int last_iteration_time = 0;
    int head_row = 8;
    int head_col = 2;
    unsigned int fruit_row;
    unsigned int fruit_col;
    unsigned int size = 3;
    bool (*game_buffer_ptr)[16];
    bool (*display_buffer_ptr)[16];
    int (*display_pixel_brightness_ptr)[16];
    Direction snake_direction = Direction::right;
    queue<SnakeElementPosition> snake_element_positions;

    void new_fruit() {
        bool valid_fruit_position = false;
        unsigned int new_fruit_row;
        unsigned int new_fruit_col;
        // Rejects fruits with the same position as a snake element
        while (!valid_fruit_position) {
            new_fruit_row = rand() % 16;
            new_fruit_col = rand() % 16;
            valid_fruit_position = true;
            // It is not possible to directly iterate over a queue. Thus, a copy must be created and repeatedly popped.
            queue<SnakeElementPosition> iterate_queue;
            iterate_queue = snake_element_positions;
            for (int i = 0; i < iterate_queue.size(); i++) {
                SnakeElementPosition snake_element_position = iterate_queue.front();
                iterate_queue.pop();
                if (snake_element_position.col == fruit_col && snake_element_position.row == fruit_row) {
                    valid_fruit_position = false;
                    break;
                }
            }
        }
        fruit_col = new_fruit_col;
        fruit_row = new_fruit_row;
        game_buffer_ptr[fruit_row][fruit_col] = 1;
    } 

    bool check_collision() {
        if (head_row == 16 || head_col == 16 || head_row == -1 || head_col == -1) {
            return true;
        }
        queue<SnakeElementPosition> iterate_queue;
        iterate_queue = snake_element_positions;
        for (int i = 0; i < iterate_queue.size(); i++) {
            SnakeElementPosition snake_element_position = iterate_queue.front();
            iterate_queue.pop();
            if (snake_element_position.col == head_col && snake_element_position.row == head_row) {
                return true;
            }
        }
        return false;
    }

    void clear_game_buffer() {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = 0;
            }
        }
    }

    void check_valid_direction(Direction new_dir) {
        if ((snake_direction == Direction::right && new_dir == Direction::left) ||
            (snake_direction == Direction::left && new_dir == Direction::right) ||
            (snake_direction == Direction::up && new_dir == Direction::down) ||
            (snake_direction == Direction::down && new_dir == Direction::up)) {
                unload();
                load();
            }
    }

    public:

    Snake(bool (*&game_buffer_ptr)[16], bool (*&display_buffer_ptr)[16], int (*&display_pixel_brightness_ptr)[16]) {
        this->game_buffer_ptr = game_buffer_ptr;
        this->display_buffer_ptr = display_buffer_ptr;
        this->display_pixel_brightness_ptr = display_pixel_brightness_ptr;
    }

    void load() override {
        clear_game_buffer();
        new_fruit();
        snake_element_positions.push({0, 8});
        snake_element_positions.push({1, 8});
        snake_element_positions.push({2, 8});
    } 

    void unload() override {
        clear_game_buffer();
        head_row = 8;
        head_col = 2;
        size = 3;
        snake_direction = Direction::right;
        last_iteration_time = 0;
        queue<SnakeElementPosition> empty;
        swap(snake_element_positions, empty);
    }

    void iterate() override {
        unsigned int current_time = millis();
        if (current_time - last_iteration_time <= iteration_delay) return;
        last_iteration_time = current_time;
        Vector2D vector = get_direction_vector(snake_direction);
        head_row += vector.dy;
        head_col += vector.dx;
        if (check_collision() || size == 256) {
            unload();
            load();
            return;
        }
        game_buffer_ptr[head_row][head_col] = 1;
        snake_element_positions.push({head_col, head_row});
        if (head_row == fruit_row && head_col == fruit_col) {
            size++;
            new_fruit();
        } else {
            SnakeElementPosition snake_tail = snake_element_positions.front();
            snake_element_positions.pop();
            game_buffer_ptr[snake_tail.row][snake_tail.col] = 0;
        }
    }

    void joystick_up() override {
        check_valid_direction(Direction::up);
        snake_direction = Direction::up;
    }

    void joystick_right() override {
        check_valid_direction(Direction::right);
        snake_direction = Direction::right;
    }

    void joystick_down() override {
        check_valid_direction(Direction::down);
        snake_direction = Direction::down;
    }

    void joystick_left() override {
        check_valid_direction(Direction::left);
        snake_direction = Direction::left;
    }

    void joystick_action() override {};
    void button1() override {};
    void button2() override {};
    void button3() override {};
    void potentiometer_change(unsigned int raw_potentiometer_input) override {};
    void rotary_encoder_clockwise() {};
    void rotary_encoder_anticlockwise() {};
};