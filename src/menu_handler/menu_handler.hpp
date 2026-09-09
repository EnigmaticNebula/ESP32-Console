#include <menu_handler/menu_graphics.hpp>
#include <string>
using namespace std;

class MenuHandler {
    private:
    unsigned int game_count = 1;
    bool (*_game_buffer_ptr)[16];

    void select_game() {
        switch (selected_game) {
            case 1:
                game_1();
                break;
        }
    }

    void game_1() {
        set_menu_graphics("game1");
    }

    void set_menu_graphics(string game_id) {
        array<array<bool, 16>, 16> menu_graphics = game_menu_graphics[game_id];
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                _game_buffer_ptr[i][j] = menu_graphics[i][j];
            }
        }
    }

    public:

    bool menu_active = true;
    unsigned int selected_game = 1;

    MenuHandler(bool (*&game_buffer_ptr)[16] ) {
        _game_buffer_ptr = game_buffer_ptr;
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
        set_menu_graphics("game1");
    }
};