#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <text_box/character_set.hpp>
#include <iostream>
using namespace std;

#include <Arduino.h>


class TextBox {
    private:
        unsigned int tl_row = 0;
        unsigned int tl_col = 0;
        unsigned int br_row = 0;
        unsigned int br_col = 0;
        int overflow_scroll_delay = 0;
        unsigned int overflow_reset_delay = 0; // How long to wait before resetting scrolling position
        unsigned int visible_text_box_width;
        unsigned int scroll_offset; // The number of pixels the scrolling text is offset by
        unsigned int text_length;
        unsigned int last_scroll_time = 0;
        unsigned int last_scroll_reset_time = 0;
        unsigned int start_scroll_delay = 0; // How long to wait before scrolling starts on reset
        bool (*game_buffer_ptr)[16];
        bool should_scroll = false;
        array<vector<bool>, 5> text_buffer = {{{}, {}, {}, {}, {}}};
        string text = "";
    public:

    bool hidden = false;
    TextBox(unsigned int tl_row, unsigned int tl_col, unsigned int br_row, unsigned int br_col, int overflow_scroll_delay, unsigned int overflow_reset_delay, unsigned int start_scroll_delay, string text) {
        this->tl_row = tl_row;
        this->tl_col = tl_col;
        this->br_row = br_row;
        this->br_col = br_col;
        this->overflow_scroll_delay = overflow_scroll_delay;
        this->text = text;
        this->overflow_reset_delay = overflow_reset_delay;
        this->start_scroll_delay = start_scroll_delay;
    }

    void init_text() {
        text_length = 0;
        should_scroll = false;
        for (int i = 0; i < 5; i++) {
            vector<bool> text_row;
            for (int j = 0; j < text.length(); j++) {
                char chr = text[j];
                
                if (character_mapping.count(chr) == 0) {
                    if (i == 0) text_length = text_length + 3;
                    text_row.insert(text_row.end(), begin(unknown[i]), end(unknown[i]));
                } else {
                    if (i == 0) text_length = text_length + character_mapping[chr][0].size();
                    array<vector<bool>, 5> display_char = character_mapping[chr];
                    text_row.insert(text_row.end(), begin(display_char[i]), end(display_char[i]));
                }
                if (j != text.length() - 1) text_row.push_back(0);
            }
            text_buffer[i] = text_row;
        }
        text_length = text_length + text.length() - 1;
        if (tl_col + text_length > 16) {
            should_scroll = true;
        }
        visible_text_box_width = 16 - tl_col;
        last_scroll_reset_time = millis(); // Ensures that scrolling is paused on startup
    }

    void refresh_text() {
        if (hidden) return;

        // Text is updated in columns instead of rows as doing it in rows causes each row to move out of sync making text harder to read
        for (int j = 0; j < 16; j++) {
            for (int i = 0; i < 5; i++) {
                if (j + tl_col > 15) break;
                if (j + scroll_offset >= text_length) {
                    game_buffer_ptr[tl_row - i][tl_col + j] = 0;
                } else {
                    game_buffer_ptr[tl_row - i][tl_col + j] = text_buffer[i][j + scroll_offset];
                };
            }
        }
        unsigned int current_time = millis();
        if (current_time - last_scroll_time >= overflow_scroll_delay && should_scroll) {
            if (scroll_offset > text_length && current_time - last_scroll_time >= overflow_reset_delay) {
                scroll_offset = 0;
                last_scroll_reset_time = current_time;
            } else if (scroll_offset <= text_length && current_time - last_scroll_reset_time >= start_scroll_delay) {
                scroll_offset++;
                last_scroll_time = current_time;
            }
        }
    }

    void change_text(string text) {
        this->text = text;
        this->scroll_offset = 0;
        init_text();
    }

    void assign_game_buffer(bool (*&game_buffer_ptr)[16]) {
        this->game_buffer_ptr = game_buffer_ptr;
    }
};