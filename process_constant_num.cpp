#include "process_constant_num.h"

typedef enum {
    state_0, state_1, state_2, state_3, state_4,
    state_5, state_6, state_err, state_end
} pcn_state;

bool right_suffix(char ch) {
    vector <char> allow_suffix = { //算术常数允许的后继符集
        ' ', '\n', '+', '-', '*', '/', '=', '>', '<', ',', '&', '|', '!', ']', ':', '?', ')', ';'
    };
    for (unsigned i = 0; i < allow_suffix.size(); i++) {
        if (ch == allow_suffix[i]) {
            return true;
        }
    }
    return false;
}

pcn_state state_shifter_0(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_1;
    else return state_err;
}
pcn_state state_shifter_1(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_1;
    else if (right_suffix(ch)) return state_end;
    else if (ch == '.') return state_2;
    else if (ch == 'e' || ch == 'E') return state_4;
    else return state_err;
}
pcn_state state_shifter_2(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_3;
    else return state_err;
}
pcn_state state_shifter_3(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_3;
    else if (right_suffix(ch)) return state_end;
    else if (ch == 'e' || ch == 'E') return state_4;
    else return state_err;
}
pcn_state state_shifter_4(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_6;
    else if (ch == '+' || ch == '-') return state_5;
    else return state_err;
}
pcn_state state_shifter_5(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_6;
    else return state_err;
}
pcn_state state_shifter_6(char ch, pcn_state current_state) {
    if (is_num(ch)) return state_6;
    else if (right_suffix(ch)) return state_end;
    else return state_err;
}

pcn_state (*pcn_state_shifter[7])(char, pcn_state) = {
    state_shifter_0,
    state_shifter_1,
    state_shifter_2,
    state_shifter_3,
    state_shifter_4,
    state_shifter_5,
    state_shifter_6
};
bool process_constant_num(string& buffer, unsigned int& cur_index, double& num) {
    pcn_state state = state_0; char ch;
    int value, exp_value, frac_width, exp_sign;
    enum {INTEGER, FLOAT} num_type;
    while (state != state_end) {
        switch (state) {
            case state_0:
                value = exp_value = frac_width = 0;
                exp_sign = 1;
                num = 0.0;
                num_type = INTEGER;
                break;
            case state_1:
                value = 10 * value + (ch - '0');
                break;
            case state_2:
                num_type = FLOAT;
                break;
            case state_3:
                value = 10 * value + (ch - '0');
                frac_width ++;
                break;
            case state_4:
                num_type = FLOAT;
                break;
            case state_5:
                if (ch == '-') exp_sign = -1;
                break;
            case state_6:
                exp_value = 10 * exp_value + (ch - '0');
                break;
            case state_err:
                return false;
        }
        ch = buffer[cur_index ++];
        state = (*pcn_state_shifter[state])(ch, state);
    }
    num = value * pow(10, exp_sign * exp_value - frac_width);
    cur_index --;
//    cout << "<num_value>: " << num << " <num_type>: ";
//    if (num_type == INTEGER) cout << "INTEGER" << endl;
//    else if (num_type == FLOAT) cout << "FLOAT" << endl;
    return true;
}
