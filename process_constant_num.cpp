#include "process_constant_num.h"

typedef enum {
    state_0, state_1, state_2, state_3, state_4,
    state_5, state_6, state_err, state_end
} pcn_state; // 所有自动机状态枚举

bool right_suffix(char ch) { // 判断ch是否为合法后继符，将来用文法符号的follow集找出
    vector <char> allow_suffix = { //算术常数允许的后继符集
        ' ', '\n', '+', '-', '*', '/', '=', '>', '<', ',', '&', '|', '!', '^', ']', ':', '?', ')', ';'
    };
    for (unsigned i = 0; i < allow_suffix.size(); i++) {
        if (ch == allow_suffix[i]) {
            return true;
        }
    }
    return false;
}

// 所有状态转移函数，参数为当前状态和当前符号，输出为下一状态，相当于自动机图中的有向边
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

// 将所有的转移函数汇总在一个函数指针数组中
pcn_state (*pcn_state_shifter[7])(char, pcn_state) = {
    state_shifter_0,
    state_shifter_1,
    state_shifter_2,
    state_shifter_3,
    state_shifter_4,
    state_shifter_5,
    state_shifter_6
};

// 算术常数处理
bool process_constant_num(string& buffer, unsigned int& cur_index, double& num) {
    // 初始化
    pcn_state state = state_0; char ch;
    int value, exp_value, frac_width, exp_sign;
    enum {INTEGER, FLOAT} num_type;

    // 运行状态机直到遇见结束状态
    while (state != state_end) {
        switch (state) {
            // 对于每个状态节点需要做的事
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
        // 读下一字符
        ch = buffer[cur_index ++];

        // 状态转移
        state = (*pcn_state_shifter[state])(ch, state);
    }
    // 合成最终的算术常数
    num = value * pow(10, exp_sign * exp_value - frac_width);
    cur_index --;
//    cout << "<num_value>: " << num << " <num_type>: ";
//    if (num_type == INTEGER) cout << "INTEGER" << endl;
//    else if (num_type == FLOAT) cout << "FLOAT" << endl;
    return true;
}
