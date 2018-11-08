#include "process_character_string.h"

typedef enum {
    state_0, state_1, state_2, state_3, state_err, state_end
} pcs_state;

pcs_state state_shifter_0(char ch, pcs_state current_state) {
    if (ch == '\\') return state_1;
    else if (ch == '\n' || ch == '\'') return state_err;
    else if (ch == '\"') return state_end;
    else return state_3;
}
pcs_state state_shifter_1(char ch, pcs_state current_state) {
    if (ch == 'n' || ch == 't' || ch == '0' ||
        ch == '\\' || ch == '\'' || ch == '\"')
        return state_2;
    else return state_err;
}
pcs_state state_shifter_2(char ch, pcs_state current_state) {
    return state_0;
}
pcs_state state_shifter_3(char ch, pcs_state current_state) {
    return state_0;
}

pcs_state (*pcs_state_shifter[4])(char, pcs_state) = {
    state_shifter_0,
    state_shifter_1,
    state_shifter_2,
    state_shifter_3
};

bool process_character_string(string& buffer, unsigned int& cur_index, string& str) {
    pcs_state state = state_0;
    char cur_ch; str = "";
    while (state != state_end) {
        switch (state) {
            case state_2:
                char ch;
                switch (cur_ch) {
                    case 'n': ch = '\n'; break;
                    case 't': ch = '\t'; break;
                    case '0': ch = '\0'; break;
                    default : ch = cur_ch; break;
                }
                str += ch;
                cur_index --;
                break;
            case state_3:
                str += cur_ch;
                cur_index --;
                break;
            case state_err:
                return false;
        }
        if (cur_index == buffer.length()) return false;
        cur_ch = buffer[cur_index ++];
        state = (*pcs_state_shifter[state])(cur_ch, state);
    }
    return true;
}
