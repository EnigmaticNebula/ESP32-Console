#include <string>
using namespace std;

class TextBox {
    private:

    public:
    unsigned int tl_row = 0;
    unsigned int tl_col = 0;
    unsigned int br_row = 0;
    unsigned int br_col = 0;
    int overflow_scroll_speed = 0;
    unsigned int overflow_reset_delay = 0;
    string text = "";

    TextBox(unsigned int tl_row, unsigned int tl_col, unsigned int br_row, unsigned int br_col, int overflow_scroll_speed, unsigned int overflow_reset_delay, string text) {
        this->tl_row = tl_row;
        this->tl_col = tl_col;
        this->br_row = br_row;
        this->br_col = br_col;
        this->overflow_scroll_speed = overflow_scroll_speed;
        this->text = text;
        this->overflow_reset_delay = overflow_reset_delay;
    }
};