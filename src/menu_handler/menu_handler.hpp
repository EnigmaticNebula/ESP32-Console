#include <menu_handler/menu_graphics.hpp>
#include <string>
#include <text_box/text_box.hpp>
#include <tuple>
#include <games\conways\conways.hpp>
using namespace std;

class MenuHandler {
    private:
    bool (*game_buffer_ptr)[16];
    int (*display_pixel_brightness_ptr)[16];

    void select_game() {
        switch (selected_game) {
            case 0:
                select_conways();
                break;
            case 1:
                select_naughts_crosses();
                break;
            case 2:
                select_snake();
                break;
            case 3:
                select_platformer();
                break;
            case 4:
                select_flappy_cube();
                break;
            case 5:
                select_pong();
                break;
            case 6:
                select_pacman();
                break;
            case 7:
                select_paint();
                break;
            case 8:
                select_stopwatch();
                break;
            case 9:
                select_timer();
                break;
            case 10:
                select_settings();
                break;
        }
    }

    void select_conways() {
        set_menu_graphics(0);
        menu_text.change_text("Conway's Game of Life");
    }

    void select_naughts_crosses() {
        set_menu_graphics(1);
        menu_text.change_text("Naughts and Crosses");
    }

    void select_snake() {
        set_menu_graphics(2);
        menu_text.change_text("Snake");
    }

    void select_platformer() {
        set_menu_graphics(3);
        menu_text.change_text("Platformer");
    }

    void select_flappy_cube() {
        set_menu_graphics(4);
        menu_text.change_text("Flappy Cube");
    }

    void select_pong() {
        set_menu_graphics(5);
        menu_text.change_text("Pong");
    }

    void select_pacman() {
        set_menu_graphics(6);
        menu_text.change_text("Pacman");
    }

    void select_paint() {
        set_menu_graphics(7);
        menu_text.change_text("Paint");
    }

    void select_stopwatch() {
        set_menu_graphics(8);
        menu_text.change_text("Stopwatch");
    }

    void select_timer() {
        set_menu_graphics(9);
        menu_text.change_text("Timer");
    }

    void select_settings() {
        set_menu_graphics(10);
        menu_text.change_text("Settings");
    }

    void set_menu_graphics(int game_id) {
        array<array<bool, 16>, 16> menu_graphics = game_menu_graphics[game_id];
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                game_buffer_ptr[i][j] = menu_graphics[15 - i][j]; // Row index is 15 - i as menu graphics are flipped to make designing easier
                display_pixel_brightness_ptr[i][j] = menu_graphics_brightness[15 - i][j];
            }
        }
    }
    TextBox menu_text{4, 0, 0, 15, 100, 250, 1000, ""};

    public:

    volatile bool menu_active = true;
    unsigned int selected_game = 0;

    MenuHandler(bool (*&game_buffer_ptr)[16], int (*&display_pixel_brightness_ptr)[16]) {
        this->game_buffer_ptr = game_buffer_ptr;
        this->display_pixel_brightness_ptr = display_pixel_brightness_ptr;
    }

    void next_game() {
        selected_game++;
        select_game();
    }

    void previous_game() {
        selected_game--;
        select_game();
    }

    void init_menu() {
        select_game();
        menu_text.init_text();
        menu_text.assign_game_buffer(game_buffer_ptr); // Game buffer is assigned after instantiation as game_buffer_ptr is nullptr during instantiation
    }

    void refresh() {
        if (menu_active) {
            menu_text.refresh_text();
        } else {
            menu_text.hidden = true;
        }
    }

    void hide_menu() {
        menu_active = false;
        menu_text.hidden = true;
    }

    void show_menu() {
        menu_active = true;
        menu_text.hidden = false;
    }
};