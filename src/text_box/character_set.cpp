#include <string>
#include <map>
#include <array>
#include <vector>
#include <text_box\character_set.hpp>
using namespace std;

array<vector<bool>, 5> upper_a = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> upper_b = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 0}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_c = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_d = {{
    {1, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 0}
}};
array<vector<bool>, 5> upper_e = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_f = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 0, 0}
}};
array<vector<bool>, 5> upper_g = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_h = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> upper_i = {{
    {1, 1, 1}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_j = {{
    {1, 1, 1}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {1, 1, 0}
}};
array<vector<bool>, 5> upper_k = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> upper_l = {{
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_m = {{
    {1, 0, 0, 0, 1}, 
    {1, 1, 0, 1, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1}
}};
array<vector<bool>, 5> upper_n = {{
    {1, 0, 0, 1}, 
    {1, 1, 0, 1}, 
    {1, 0, 1, 1}, 
    {1, 0, 0, 1}, 
    {1, 0, 0, 1}
}};
array<vector<bool>, 5> upper_o = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_p = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 0, 0}
}};
array<vector<bool>, 5> upper_q = {{
    {0, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 1, 1}
}};
array<vector<bool>, 5> upper_r = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> upper_s = {{
    {0, 1, 1}, 
    {1, 0, 0}, 
    {0, 1, 0}, 
    {0, 0, 1}, 
    {1, 1, 0}
}};
array<vector<bool>, 5> upper_t = {{
    {1, 1, 1}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {0, 1, 0}
}};
array<vector<bool>, 5> upper_u = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> upper_v = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {0, 1, 0}
}};
array<vector<bool>, 5> upper_w = {{
    {1, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 1, 0, 1, 1}, 
    {1, 0, 0, 0, 1}
}};
array<vector<bool>, 5> upper_x = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> upper_y = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 1, 0}, 
    {0, 1, 0}
}};
array<vector<bool>, 5> upper_z = {{
    {1, 1, 1}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_a = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_b = {{
    {0, 0, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_c = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_d = {{
    {0, 0, 0}, 
    {0, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_e = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_f = {{
    {0, 0, 0}, 
    {0, 1, 1}, 
    {0, 1, 0}, 
    {1, 1, 1}, 
    {0, 1, 0}
}};
array<vector<bool>, 5> lower_g = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_h = {{
    {0, 0, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> lower_i = {{
    {0}, 
    {1}, 
    {0}, 
    {1}, 
    {1}
}};
array<vector<bool>, 5> lower_j = {{
    {0, 0}, 
    {0, 1}, 
    {0, 0}, 
    {0, 1}, 
    {1, 1}
}};
array<vector<bool>, 5> lower_k = {{
    {0, 0, 0}, 
    {1, 0, 0}, 
    {1, 0, 1}, 
    {1, 1, 0}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> lower_l = {{
    {0, 0}, 
    {1, 0}, 
    {1, 0}, 
    {1, 0}, 
    {1, 1}
}};
array<vector<bool>, 5> lower_m = {{
    {0, 0, 0, 0, 0}, 
    {0, 1, 0, 1, 0}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 1}
}};
array<vector<bool>, 5> lower_n = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> lower_o = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_p = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 0}
}};
array<vector<bool>, 5> lower_q = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}
}};
array<vector<bool>, 5> lower_r = {{
    {0, 0}, 
    {1, 1}, 
    {1, 0}, 
    {1, 0}, 
    {1, 0}
}};
array<vector<bool>, 5> lower_s = {{
    {0, 0, 0}, 
    {0, 1, 1}, 
    {1, 1, 0}, 
    {0, 1, 1}, 
    {1, 1, 0}
}};
array<vector<bool>, 5> lower_t = {{
    {0, 0}, 
    {1, 0}, 
    {1, 1}, 
    {1, 0}, 
    {1, 1}
}};
array<vector<bool>, 5> lower_u = {{
    {0, 0, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_v = {{
    {0, 0, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {0, 1, 0}
}};
array<vector<bool>, 5> lower_w = {{
    {0, 0, 0, 0, 0}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {0, 1, 0, 1, 0}
}};
array<vector<bool>, 5> lower_x = {{
    {0, 0, 0}, 
    {1, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 1}, 
    {1, 0, 1}
}};
array<vector<bool>, 5> lower_y = {{
    {0, 0, 0}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}
}};
array<vector<bool>, 5> lower_z = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> zero = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> one = {{
    {0, 1, 0}, 
    {1, 1, 0}, 
    {0, 1, 0}, 
    {0, 1, 0}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> two = {{
    {1, 1, 0}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> three = {{
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> four = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {0, 0, 1}
}};

array<vector<bool>, 5> five = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 0}, 
    {0, 0, 1}, 
    {1, 1, 0}
}};

array<vector<bool>, 5> six = {{
    {1, 1, 1}, 
    {1, 0, 0}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> seven = {{
    {1, 1, 1}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}
}};

array<vector<bool>, 5> eight = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> nine = {{
    {1, 1, 1}, 
    {1, 0, 1}, 
    {1, 1, 1}, 
    {0, 0, 1}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> exclamation = {{
    {1}, 
    {1}, 
    {1}, 
    {0}, 
    {1}
}};

array<vector<bool>, 5> question = {{
    {1, 1, 1}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {0, 0, 0}, 
    {0, 1, 0}
}};

array<vector<bool>, 5> comma = {{
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 1, 0}, 
    {1, 1, 0}
}};

array<vector<bool>, 5> forward_slash = {{
    {0, 0, 1}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 0, 0}
}};

array<vector<bool>, 5> backward_slash = {{
    {1, 0, 0}, 
    {1, 0, 0}, 
    {0, 1, 0}, 
    {0, 0, 1}, 
    {0, 0, 1}
}};

array<vector<bool>, 5> equals = {{
    {0, 0, 0}, 
    {1, 1, 1}, 
    {0, 0, 0}, 
    {1, 1, 1}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> asterisk = {{
    {0, 0, 0}, 
    {1, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 1}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> plus_symbol = {{
    {0, 0, 0}, 
    {0, 1, 0}, 
    {1, 1, 1}, 
    {0, 1, 0}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> minus_symbol = {{
    {0, 0, 0}, 
    {0, 0, 0}, 
    {1, 1, 1}, 
    {0, 0, 0}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> double_quotation = {{
    {1, 0, 1}, 
    {1, 0, 1}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> hashtag = {{
    {0, 1, 0, 1, 0}, 
    {1, 1, 1, 1, 1}, 
    {0, 1, 0, 1, 0}, 
    {1, 1, 1, 1, 1}, 
    {0, 1, 0, 1, 0}
}};

array<vector<bool>, 5> dollar = {{
    {0, 1, 0}, 
    {0, 1, 1}, 
    {1, 1, 0}, 
    {0, 1, 1}, 
    {0, 1, 0}
}};

array<vector<bool>, 5> percentage = {{
    {1, 0, 1}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {1, 0, 1}
}};

array<vector<bool>, 5> apostrophe = {{
    {1}, 
    {1}, 
    {0}, 
    {0}, 
    {0}
}};

array<vector<bool>, 5> open_bracket = {{
    {0, 1}, 
    {1, 0}, 
    {1, 0}, 
    {1, 0}, 
    {0, 1}
}};

array<vector<bool>, 5> closed_bracket = {{
    {1, 0}, 
    {0, 1}, 
    {0, 1}, 
    {0, 1}, 
    {1, 0}
}};

array<vector<bool>, 5> open_squared_bracket = {{
    {1, 1}, 
    {1, 0}, 
    {1, 0}, 
    {1, 0}, 
    {1, 1}
}};

array<vector<bool>, 5> closed_squared_bracket = {{
    {1, 1}, 
    {0, 1}, 
    {0, 1}, 
    {0, 1}, 
    {1, 1}
}};

array<vector<bool>, 5> open_curly_bracket = {{
    {0, 1, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {0, 1, 0}, 
    {0, 1, 1}
}};

array<vector<bool>, 5> closed_curly_bracket = {{
    {1, 1, 0}, 
    {0, 1, 0}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 1, 0}
}};

array<vector<bool>, 5> colon = {{
    {1, 0}, 
    {0, 0}, 
    {0, 0}, 
    {0, 0}, 
    {1, 0}
}};

array<vector<bool>, 5> semi_colon = {{
    {0, 1, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 1, 0}, 
    {1, 0, 0}
}};

array<vector<bool>, 5> period = {{
    {0},
    {0},
    {0},
    {0},
    {1}
}};

array<vector<bool>, 5> less_than = {{
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}, 
    {0, 1, 0}, 
    {0, 0, 1}
}};

array<vector<bool>, 5> greater_than = {{
    {1, 0, 0}, 
    {0, 1, 0}, 
    {0, 0, 1}, 
    {0, 1, 0}, 
    {1, 0, 0}
}};

array<vector<bool>, 5> up_arrow = {{
    {0, 1, 0}, 
    {1, 0, 1}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}
}};

array<vector<bool>, 5> underscore = {{
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}, 
    {1, 1, 1}
}};

array<vector<bool>, 5> space = {{
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}
}};

array<vector<bool>, 5> tilde = {{
    {0, 0, 0, 0},
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}};

array<vector<bool>, 5> pound = {{
    {0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1}
}};


array<vector<bool>, 5> unknown = {{
    {1, 1, 1}, 
    {1, 1, 1}, 
    {1, 1, 1}, 
    {1, 1, 1}, 
    {1, 1, 1}
}};

map<char, array<vector<bool>, 5>> character_mapping = {
    {'a', lower_a},
    {'b', lower_b},
    {'c', lower_c},
    {'d', lower_d},
    {'e', lower_e},
    {'f', lower_f},
    {'g', lower_g},
    {'h', lower_h},
    {'i', lower_i},
    {'j', lower_j},
    {'k', lower_k},
    {'l', lower_l},
    {'m', lower_m},
    {'n', lower_n},
    {'o', lower_o},
    {'p', lower_p},
    {'q', lower_q},
    {'r', lower_r},
    {'s', lower_s},
    {'t', lower_t},
    {'u', lower_u},
    {'v', lower_v},
    {'w', lower_w},
    {'x', lower_x},
    {'y', lower_y},
    {'z', lower_z},
    {'A', upper_a},
    {'B', upper_b},
    {'C', upper_c},
    {'D', upper_d},
    {'E', upper_e},
    {'F', upper_f},
    {'G', upper_g},
    {'H', upper_h},
    {'I', upper_i},
    {'J', upper_j},
    {'K', upper_k},
    {'L', upper_l},
    {'M', upper_m},
    {'N', upper_n},
    {'O', upper_o},
    {'P', upper_p},
    {'Q', upper_q},
    {'R', upper_r},
    {'S', upper_s},
    {'T', upper_t},
    {'U', upper_u},
    {'V', upper_v},
    {'W', upper_w},
    {'X', upper_x},
    {'Y', upper_y},
    {'Z', upper_z},
    {'0', zero},
    {'1', one},
    {'2', two},
    {'3', three},
    {'4', four},
    {'5', five},
    {'6', six},
    {'7', seven},
    {'8', eight},
    {'9', nine},
    {'!', exclamation},
    {'?', question},
    {',', comma},
    {'/', forward_slash},
    {'\\', backward_slash},
    {'=', equals},
    {'*', asterisk},
    {'+', plus_symbol},
    {'-', minus_symbol},
    {'\"', double_quotation},
    {'#', hashtag},
    {'$', dollar},
    {'%', percentage},
    {'\'', apostrophe},
    {'(', open_bracket},
    {')', closed_bracket},
    {'[', open_squared_bracket},
    {']', closed_squared_bracket},
    {'{', open_curly_bracket},
    {'}', closed_curly_bracket},
    {':', colon},
    {';', semi_colon},
    {'.', period},
    {'<', less_than},
    {'>', greater_than},
    {'^', up_arrow},
    {'_', underscore},
    {'~', tilde},
    {' ', space}
};