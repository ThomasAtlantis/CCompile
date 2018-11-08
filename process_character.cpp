#include "process_character.h"

typedef enum {
    state_0, state_1, state_2, state_3, state_err, state_end
} pc_state;

pc_state state_shifter_0(char ch, pc_state current_state) {
    if (ch == '\\') return state_1;
    else if (ch == '\n' || ch == '\'' || ch == '\"') return state_err;
    else return state_3;
}
pc_state state_shifter_1(char ch, pc_state current_state) {
    if (ch == 'n' || ch == 't' || ch == '0' ||
        ch == '\\' || ch == '\'' || ch == '\"')
        return state_2;
    else return state_err;
}
pc_state state_shifter_2(char ch, pc_state current_state) {
    if (ch == '\'') return state_end;
    else return state_err;
}
pc_state state_shifter_3(char ch, pc_state current_state) {
    if (ch == '\'') return state_end;
    else return state_err;
}

pc_state (*pc_state_shifter[4])(char, pc_state) = {
    state_shifter_0,
    state_shifter_1,
    state_shifter_2,
    state_shifter_3
};

bool process_character(string& buffer, unsigned int& cur_index, char& ch) {
    pc_state state = state_0;
    char cur_ch;
    while (state != state_end) {
        switch (state) {
            case state_2:
                switch (cur_ch) {
                    case 'n': ch = '\n'; break;
                    case 't': ch = '\t'; break;
                    case '0': ch = '\0'; break;
                    default : ch = cur_ch; break;
                }
                break;
            case state_3:
                ch = cur_ch;
                break;
            case state_err:
                return false;
        }
        cur_ch = buffer[cur_index ++];
        state = (*pc_state_shifter[state])(cur_ch, state);
    }
    return true;
}
