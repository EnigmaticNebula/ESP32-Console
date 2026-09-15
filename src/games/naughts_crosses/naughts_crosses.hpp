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
        {"", "", ""}
    };
    bool crosses_starts = false;
    bool crosses_turn = false;
    bool selection_change = false;
    unsigned int selected_row = 0;
    unsigned int selected_column = 0;
    unsigned int prev_selected_row = 0;
    unsigned int prev_selected_column = 0;
    unsigned int box_tl_row = 1;
    unsigned int box_tl_col = 1;
    unsigned int prev_box_tl_row = 1;
    unsigned int prev_box_tl_col = 1;

    void clear_game_buffer() {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = 0;
            }
        }
    }

    void select_cross_square() {
        game_buffer_ptr[box_tl_row][box_tl_col + 1] = 1;
        game_buffer_ptr[box_tl_row][box_tl_col + 2] = 1;
        game_buffer_ptr[box_tl_row + 3][box_tl_col + 1] = 1;
        game_buffer_ptr[box_tl_row + 3][box_tl_col + 2] = 1;
        game_buffer_ptr[box_tl_row + 1][box_tl_col] = 1;
        game_buffer_ptr[box_tl_row + 2][box_tl_col] = 1;
        game_buffer_ptr[box_tl_row + 1][box_tl_col + 3] = 1;
        game_buffer_ptr[box_tl_row + 2][box_tl_col + 3] = 1;
        display_pixel_brightness_ptr[box_tl_row][box_tl_col + 1] = 1;
        display_pixel_brightness_ptr[box_tl_row][box_tl_col + 2] = 1;
        display_pixel_brightness_ptr[box_tl_row + 3][box_tl_col + 1] = 1;
        display_pixel_brightness_ptr[box_tl_row + 3][box_tl_col + 2] = 1;
        display_pixel_brightness_ptr[box_tl_row + 1][box_tl_col] = 1;
        display_pixel_brightness_ptr[box_tl_row + 2][box_tl_col] = 1;
        display_pixel_brightness_ptr[box_tl_row + 1][box_tl_col + 3] = 1;
        display_pixel_brightness_ptr[box_tl_row + 2][box_tl_col + 3] = 1;
    }

    void select_naught_square() {
        game_buffer_ptr[box_tl_row][box_tl_col] = 1;
        game_buffer_ptr[box_tl_row][box_tl_col + 3] = 1;
        game_buffer_ptr[box_tl_row + 1][box_tl_col + 1] = 1;
        game_buffer_ptr[box_tl_row + 1][box_tl_col + 2] = 1;
        game_buffer_ptr[box_tl_row + 2][box_tl_col + 1] = 1;
        game_buffer_ptr[box_tl_row + 2][box_tl_col + 2] = 1;
        game_buffer_ptr[box_tl_row + 3][box_tl_col] = 1;
        game_buffer_ptr[box_tl_row + 3][box_tl_col + 3] = 1;
        display_pixel_brightness_ptr[box_tl_row][box_tl_col] = 1;
        display_pixel_brightness_ptr[box_tl_row][box_tl_col + 3] = 1;
        display_pixel_brightness_ptr[box_tl_row + 1][box_tl_col + 1] = 1;
        display_pixel_brightness_ptr[box_tl_row + 1][box_tl_col + 2] = 1;
        display_pixel_brightness_ptr[box_tl_row + 2][box_tl_col + 1] = 1;
        display_pixel_brightness_ptr[box_tl_row + 2][box_tl_col + 2] = 1;
        display_pixel_brightness_ptr[box_tl_row + 3][box_tl_col] = 1;
        display_pixel_brightness_ptr[box_tl_row + 3][box_tl_col + 3] = 1;
    }

    void select_unused_square() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                game_buffer_ptr[i + box_tl_row][j + box_tl_col] = 1;
                display_pixel_brightness_ptr[i + box_tl_row][j + box_tl_col] = 1;
            }
        }
    }

    void check_win_conditions() {
        if (check_row_win()) return;
        if (check_col_win()) return;
        if (check_diagonal_win()) return;
        check_stalemate();
    }

    bool check_row_win() {
        for (int row = 0; row < 3; row++) {
            unsigned int naught_count = 0;
            unsigned int cross_count = 0;
            for (int col = 0; col < 3; col++) {
                if (game_map[row][col] == "x") cross_count++;
                if (game_map[row][col] == "o") naught_count++; 
            }
            if (naught_count == 3) {
                naughts_win("row", row);
                return true;
            } else if (cross_count == 3) {
                crosses_win("row", row);
                return true;
            }
        }
        return false;
    }

    bool check_col_win() {
        for (int col = 0; col < 3; col++) {
            unsigned int naught_count = 0;
            unsigned int cross_count = 0;
            for (int row = 0; row < 3; row++) {
                if (game_map[row][col] == "x") cross_count++;
                if (game_map[row][col] == "o") naught_count++; 
            }
            if (naught_count == 3) {
                naughts_win("column", col);
                return true;
            } else if (cross_count == 3) {
                crosses_win("column", col);
                return true;
            }
        }
        return false;
    }

    bool check_diagonal_win() {
        unsigned int naught_count = 0;
        unsigned int cross_count = 0;
        
        for (int i = 0; i < 3; i++) {
            if (game_map[i][i] == "x") cross_count++;
            if (game_map[i][i] == "o") naught_count++;
        }
        if (naught_count == 3) {
            naughts_win("forwards_diagonal");
            return true;
        } else if (cross_count == 3) {
            crosses_win("forwards_diagonal");
            return true;
        }
        naught_count = 0;
        cross_count = 0;
        for (int i = 0; i < 3; i++) {
            if (game_map[2 - i][i] == "x") cross_count++;
            if (game_map[2 - i][i] == "o") naught_count++;
        }
        if (naught_count == 3) {
            naughts_win("backwards_diagonal");
            return true;
        } else if (cross_count == 3) {
            crosses_win("backwards_diagonal");
            return true;
        }
        return false;
    }

    void naughts_win(String win_type, int win_pos=-1) {
        dim_all_squares();
        cross_out_winning_line(win_type, win_pos);
        crosses_starts = false;
    }

    void crosses_win(String win_type, int win_pos=-1) {
        dim_all_squares();
        cross_out_winning_line(win_type, win_pos);
        crosses_starts = true;
    }

    void check_stalemate() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (game_map[i][j] == "") return;
            }
        }
        dim_all_squares();
    }

    void dim_all_squares() {
        for (int map_row = 0; map_row < 3; map_row++) {
            unsigned int square_tl_row = 1 + (map_row * 5);
            for (int map_col = 0; map_col < 3; map_col++) {
                unsigned int square_tl_col = 1 + (map_col * 5);
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        display_pixel_brightness_ptr[square_tl_row + i][square_tl_col + j] = 1;
                    }
                }
            }
        }
    }

    void cross_out_winning_line(String win_type, int win_pos) {
        if (win_type == "row") {
            for (int i = 1; i < 15; i++) {
                game_buffer_ptr[2 + (win_pos * 5)][i] = 1;
                display_pixel_brightness_ptr[2 + (win_pos * 5)][i] = 7;
                game_buffer_ptr[3 + (win_pos * 5)][i] = 1;
                display_pixel_brightness_ptr[3 + (win_pos * 5)][i] = 7;
            }
        } else if (win_type == "column") {
            for (int i = 1; i < 15; i++) {
                game_buffer_ptr[i][2 + (win_pos * 5)] = 1;
                display_pixel_brightness_ptr[i][2 + (win_pos * 5)] = 7;
                game_buffer_ptr[i][3 + (win_pos * 5)] = 1;
                display_pixel_brightness_ptr[i][3 + (win_pos * 5)] = 7;
            }  
        } else if (win_type == "forwards_diagonal") {
            // Pixels at the start and end of the diagonal are set manually to round off the ends of the diagonals
            game_buffer_ptr[1][1] = 1;
            game_buffer_ptr[1][2] = 1;
            game_buffer_ptr[14][14] = 1;
            game_buffer_ptr[14][13] = 1;
            display_pixel_brightness_ptr[1][1] = 7;
            display_pixel_brightness_ptr[1][2] = 7;
            display_pixel_brightness_ptr[14][14] = 7;
            display_pixel_brightness_ptr[14][13] = 7;
            for (int i = 2; i < 14; i++) {
                for (int j = 0; j < 3; j++) {
                    game_buffer_ptr[i][i - 1 + j] = 1;
                    display_pixel_brightness_ptr[i][i - 1 + j] = 7;
                }
            }
        } else {
            game_buffer_ptr[14][1] = 1;
            game_buffer_ptr[14][2] = 1;
            game_buffer_ptr[1][14] = 1;
            game_buffer_ptr[1][13] = 1;
            display_pixel_brightness_ptr[14][1] = 7;
            display_pixel_brightness_ptr[14][2] = 7;
            display_pixel_brightness_ptr[1][14] = 7;
            display_pixel_brightness_ptr[1][13] = 7;
            for (int i = 2; i < 14; i++) {
                for (int j = 0; j < 3; j++) {
                    game_buffer_ptr[15 - i][i - 1 + j] = 1;
                    display_pixel_brightness_ptr[15 - i][i - 1 + j] = 7;
                }
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
        select_unused_square();
    }

    void unload() override {
        clear_game_buffer();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                game_map[i][j] = "";
            }
        }
        crosses_starts = false;
        crosses_turn = false;
        selection_change = false;
        selected_row = 0;
        selected_column = 0;
        prev_selected_row = 0;
        prev_selected_column = 0;
        box_tl_row = 1;
        box_tl_col = 1;
        prev_box_tl_row = 1;
        prev_box_tl_col = 1;
    }

    void iterate() override {
        if (selection_change) {
            if (game_map[selected_row][selected_column] == "x") {
                select_cross_square();
            } else if (game_map[selected_row][selected_column] == "o") {
                select_naught_square();
            } else {
                select_unused_square();
            }
            selection_change = false;
            if (game_map[prev_selected_row][prev_selected_column] == "x") {
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        game_buffer_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = cross_graphics[row][col];
                        display_pixel_brightness_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = 7;
                    }
                }
            } else if (game_map[prev_selected_row][prev_selected_column] == "o") {
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        game_buffer_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = naught_graphics[row][col];
                        display_pixel_brightness_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = 7;
                    }
                }
            } else {
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        game_buffer_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = 0;
                        display_pixel_brightness_ptr[(prev_box_tl_row) + row][(prev_box_tl_col) + col] = 7;
                    }
                }
            }
        }
        check_win_conditions();
    }

    void joystick_up() override {
        if (selected_row == 2) return;
        prev_selected_row = selected_row;
        prev_selected_column = selected_column;
        prev_box_tl_row = box_tl_row;
        prev_box_tl_col = box_tl_col;
        selected_row++;
        box_tl_row = 1 + (selected_row * 5);
        box_tl_col = 1 + (selected_column * 5);
        selection_change = true;
    }

    void joystick_down() override {
        if (selected_row == 0) return;
        prev_selected_row = selected_row;
        prev_selected_column = selected_column;
        prev_box_tl_row = box_tl_row;
        prev_box_tl_col = box_tl_col;
        selected_row--;
        box_tl_row = 1 + (selected_row * 5);
        box_tl_col = 1 + (selected_column * 5);
        selection_change = true;
    }

    void joystick_right() override {
        if (selected_column == 2) return;
        prev_selected_row = selected_row;
        prev_selected_column = selected_column;
        prev_box_tl_row = box_tl_row;
        prev_box_tl_col = box_tl_col;
        selected_column++;
        box_tl_row = 1 + (selected_row * 5);
        box_tl_col = 1 + (selected_column * 5);
        selection_change = true;
    }

    void joystick_left() override {
        if (selected_column == 0) return;
        prev_selected_row = selected_row;
        prev_selected_column = selected_column;
        prev_box_tl_row = box_tl_row;
        prev_box_tl_col = box_tl_col;
        selected_column--;
        box_tl_row = 1 + (selected_row * 5);
        box_tl_col = 1 + (selected_column * 5);
        selection_change = true;
    }

    void button1() override {
        if (game_map[selected_row][selected_column] != "") return;

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

    void button2() override {
        clear_game_buffer();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                game_map[i][j] = "";
            }
        }
        crosses_turn = crosses_starts;
        selection_change = false;
        selected_row = 0;
        selected_column = 0;
        prev_selected_row = 0;
        prev_selected_column = 0;
        box_tl_row = 1;
        box_tl_col = 1;
        prev_box_tl_row = 1;
        prev_box_tl_col = 1;
        load();
    };
    void button3() override {};
    void rotary_encoder_clockwise() override {};
    void rotary_encoder_anticlockwise() override {};
    void joystick_action() override {};
    void potentiometer_change(unsigned int raw_potentiometer_input) override {};
};