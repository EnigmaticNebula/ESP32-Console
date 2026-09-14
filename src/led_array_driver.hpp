#include <Arduino.h>
#include <pin_definitions.hpp>
#include <string>
#include <iostream>
using namespace std;

class LedDriver {
    private:

    bool (*_display_buffer)[16];
    int (*display_pixel_brightness_ptr)[16];

    void high_side_send_bit(int bit) {
        digitalWrite(HIGH_SIDE_SERIAL_PIN, bit);
        digitalWrite(HIGH_SIDE_SRCLK_PIN, LOW);
        digitalWrite(HIGH_SIDE_SRCLK_PIN, HIGH);
        digitalWrite(HIGH_SIDE_SRCLK_PIN, LOW);
    }

    void low_side_send_bit(int bit) {
        digitalWrite(LOW_SIDE_SERIAL_PIN, bit);
        delayMicroseconds(1);
        digitalWrite(LOW_SIDE_CLK_PIN, LOW);
        delayMicroseconds(1);
        digitalWrite(LOW_SIDE_CLK_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(LOW_SIDE_CLK_PIN, LOW);
    }

    void select_column( int row ) {
        for (int i = 0; i < 16; i++) {
            if (i == row) {
                high_side_send_bit(HIGH);
            } else {
                high_side_send_bit(LOW);
            }
        }
    }

    void trigger_latches() {
        digitalWrite(LOW_SIDE_LATCH_PIN, HIGH);
        digitalWrite(HIGH_SIDE_RCLK_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(LOW_SIDE_LATCH_PIN, LOW);
        digitalWrite(HIGH_SIDE_RCLK_PIN, LOW);   
    }

    public:

    LedDriver(bool (*&display_buffer)[16], int (*&display_pixel_brightness_ptr)[16]) {
        _display_buffer = display_buffer;
        this->display_pixel_brightness_ptr = display_pixel_brightness_ptr;
    }

    void refresh() {
        int temp_pixel_display_brightness[16][16] = {
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        };
        int (*temp_display_pixel_brightness_ptr)[16] = temp_pixel_display_brightness;
        memcpy(temp_display_pixel_brightness_ptr, display_pixel_brightness_ptr, 16*16*sizeof(int));
        for (int k = 0; k < 7; k++) {
            for (int i = 0; i < 16; i++) {
                select_column(i);
                // Refresh can theoretically be called whilst cell_states is briefly a nullptr whilst swapping sim and display buffer pointers
                if (_display_buffer == nullptr) {
                    return;
                }
                for (int j = 0; j < 16; j++) {
                    int pixel_brightness = temp_pixel_display_brightness[j][i];
                    if (pixel_brightness == 0) {
                        low_side_send_bit(0);
                    } else {
                        low_side_send_bit(_display_buffer[j][i]);
                        temp_pixel_display_brightness[j][i] = pixel_brightness - 1;
                    }
                }
                trigger_latches();
                for (int j = 0; j < 16; j++) {
                    low_side_send_bit(0);
                }
                trigger_latches();
            }
            // This causes no rows to be selected. This is done because there is a slight delay before each refresh.
            // If it is not reset, row 16 is on for a longer period of time than other rows as row 16 would still be active during the delay.
            select_column(17);
            trigger_latches();
        }
    }
};