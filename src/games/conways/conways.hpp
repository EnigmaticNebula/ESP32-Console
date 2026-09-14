#pragma once

#include <Arduino.h>
#include <vector>
#include <array>
#include <games\game.hpp>
#include <game_utilities\cursor.hpp>

class Conways : public Game {
public:
    Conways(bool (*&game_buffer_ptr)[16], bool (*&display_buffer_ptr)[16]) {
        this->game_buffer_ptr = game_buffer_ptr;
        this->display_buffer_ptr = display_buffer_ptr;
    }

    void iterate() override {
        /*
        Game buffer is cleared in the iteration loop instead of the button2 method.
        This is because the method is called as part of an interrupt service routine,
        which means that the buffer could be cleared during a memcpy operation
        between game buffer pointer and display buffer pointer.
        This could cause unexpected behaivour and flickering.      
        */ 

        if (cleared) {
            cleared = false;
            clear_game_buffer();
        }
        cursor_ptr->refresh_cursor_position();
        unsigned int current_time = millis();
        if ((!paused && current_time - last_iteration_time >= iteration_delay) || manual_iteration) {
            std::vector<std::array<int, 2>> cells_to_kill;
            std::vector<std::array<int, 2>> cells_to_revive;
            for (int row = 0; row < 16; row++) {
                for (int column = 0; column < 16; column++) {
                    bool (current_cell_state) = game_buffer_ptr[row][column];
                    int alive_neighbours = get_alive_neighbour_count(row, column);
                    if (alive_neighbours < 2 || alive_neighbours > 3) {
                        cells_to_kill.push_back(std::array<int, 2> {row, column});
                        continue;
                    }
                    if (alive_neighbours == 3 && (!current_cell_state)) {
                        cells_to_revive.push_back(std::array<int, 2> {row, column});
                    }
                }
            }
            kill_cells(cells_to_kill);
            revive_cells(cells_to_revive);
            manual_iteration = false;
            last_iteration_time = current_time;
        }
    }

    void button1() override {
        if (!paused) {
            paused = true;
            cursor_ptr->game_paused = true;
        } else {
            paused = false;
            cursor_ptr->game_paused = false;
        }
    }

    void button2() override {
        cleared = true;
    }

    void button3() override {
        manual_iteration = true;
    }

    void joystick_up() override {
        cursor_ptr->move_up();
    }

    void joystick_right() override {
        cursor_ptr->move_right();
    }

    void joystick_down() override {
        cursor_ptr->move_down();
    }

    void joystick_left() override {
        cursor_ptr->move_left();
    }

    void joystick_action() override {
        cursor_ptr->centre_select();
    }

    void potentiometer_change(unsigned int raw_potentiometer_input) override {
        iteration_delay = ::map(raw_potentiometer_input, 0, 4095, 1000, 0);
    }

    void load() override {
        cursor_ptr = new Cursor(display_buffer_ptr, game_buffer_ptr);
        clear_game_buffer();
    }

    void unload() override {
        delete cursor_ptr;
        cursor_ptr = nullptr;
        paused = false;
        cleared = false;
        manual_iteration = false;
        last_iteration_time = 0;
        clear_game_buffer();
    }

    // Unused inherited methods redefined here to make linker happy
    void rotary_encoder_clockwise() override {};
    void rotary_encoder_anticlockwise() override {};

private:

    unsigned int iteration_delay;
    bool paused = false;
    bool (*game_buffer_ptr)[16];
    bool (*display_buffer_ptr)[16];
    bool cleared = false;
    bool manual_iteration = false;
    unsigned int last_iteration_time = 0;

    Cursor* cursor_ptr = nullptr;

    int get_alive_neighbour_count(int row, int column) {
        int alive_neighbours = 0;
        for (int neighbour_row = row - 1; neighbour_row <= row + 1; neighbour_row++) {
            if (neighbour_row == -1 || neighbour_row == 16) continue;
            for (int neighbour_column = column - 1; neighbour_column <= column+1; neighbour_column++) {
                if ((neighbour_row == row && neighbour_column == column) || neighbour_column == -1 || neighbour_column == 16) continue;
                bool neighbour_cell_state = game_buffer_ptr[neighbour_row][neighbour_column];
                if (neighbour_cell_state) {
                    alive_neighbours++;
                }
            }
        }
        return alive_neighbours;
    }

    void kill_cells(std::vector<std::array<int, 2>> cells_to_kill) {
        for (int i = 0; i < cells_to_kill.size(); i++) {
            std::array<int, 2> cell_coordinates = cells_to_kill[i];
            int row = cell_coordinates[0];
            int column = cell_coordinates[1];
            game_buffer_ptr[row][column] = 0;
        };
    }

    void revive_cells(std::vector<std::array<int, 2>> cells_to_revive) {
        for (int i = 0; i < cells_to_revive.size(); i++) {
            std::array<int, 2> cell_coordinates = cells_to_revive[i];
            int row = cell_coordinates[0];
            int column = cell_coordinates[1];
            game_buffer_ptr[row][column] = 1;
        };
    }

    void clear_game_buffer() {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = 0;
            }
        }
    }
};