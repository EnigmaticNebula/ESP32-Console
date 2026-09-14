#pragma once
#include <games\game.hpp>
#include <array>
#include <Arduino.h>
#include <string>
#include <games\naughts_crosses\naughts_crosses_graphics.hpp>
using namespace std;

class NaughtsCrosses : public Game {
    private:
    bool (*game_buffer_ptr)[16];
    bool (*display_buffer_ptr)[16];
    int (*display_pixel_brightness_ptr)[16];
    String game_map[3][3] = {
        {"", "", ""},
        {"", "", ""},
        {"", "", ""},
    };
    bool crosses_starts = false;
    bool crosses_turn = false;
    bool selection_change = false;
    unsigned int selected_row = 0;
    unsigned int selected_column = 0;


    void clear_game_buffer() {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = 0;
            }
        }
    }

    public:
    
    NaughtsCrosses(bool (*&game_buffer_ptr)[16], bool (*&display_buffer_ptr)[16], int (*&display_pixel_brightness_ptr)[16]) {
        this->game_buffer_ptr = game_buffer_ptr;
        this->display_buffer_ptr = display_buffer_ptr;
        this->display_pixel_brightness_ptr = display_pixel_brightness_ptr;
    }

    void load() override {
        clear_game_buffer();
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = background_graphics[15 - i][j];
                display_pixel_brightness_ptr[i][j] = background_graphics_brightness[15 - i][j];
            }
        }
    }

    void unload() override {
        clear_game_buffer();
    }

    void iterate() override {

    }

    void joystick_up() override {
        if (selected_row == 2) return;
        selected_row++; 
        selection_change = true;
    }

    void joystick_down() override {
        if (selected_row == 0) return;
        selected_row--;
        selection_change = true;
    }

    void joystick_right() override {
        if (selected_column == 3) return;
        selected_column++;
        selection_change = true;
    }

    void joystick_left() override {
        if (selected_column == 0) return;
        selected_column--;
        selection_change = true;
    }

    void button1() override {
        if (game_map[selected_row][selected_column] != "") return;
        unsigned int box_tl_row = 1 + (selected_row * 5);
        unsigned int box_tl_col = 1 + (selected_column * 5);
        array<array<bool, 4>, 4> drawn_symbol;
        if (crosses_turn) {
            drawn_symbol = cross_graphics; 
            game_map[selected_row][selected_column] = "x";
            crosses_turn = false;
        } else {
            drawn_symbol = naught_graphics;
            game_map[selected_row][selected_column] = "o";
            crosses_turn = true;
        }
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                game_buffer_ptr[(box_tl_row) + row][(box_tl_col) + col] = drawn_symbol[row][col];
            }
        }
    }

    void button2() override {};
    void button3() override {};
    void rotary_encoder_clockwise() override {};
    void rotary_encoder_anticlockwise() override {};
    void joystick_action() override {};
    void potentiometer_change(unsigned int raw_potentiometer_input) override {};
};