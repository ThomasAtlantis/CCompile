#include "LL1.h"

// LL1构造函数，初始化成员变量
LL1::LL1(Grammar& grammar): G(grammar) {
    // 注意在此程序中必须保证“#”在第一位
    available = false;
    if (is_available()) { // 检验文法的合法性
        available = true;

        // 计算栈顶符号列表stack_tops和当前符号列表currents
        currents.push_back("#");
        stack_tops.push_back("#");
        for (Grammar::Production_iter iter_1 = grammar.G.begin(); iter_1 != grammar.G.end(); iter_1++) {
            stack_tops.push_back(iter_1->first); // 将非终结符加入栈顶符列表
            for (Grammar::Right_list_iter iter_2 = iter_1->second.begin(); iter_2 != iter_1->second.end(); iter_2 ++) {
                for (Grammar::Right_symbol_iter iter_3 = iter_2->begin(); iter_3 != iter_2->end(); iter_3 ++) {
                    if (grammar.symbol_type(*iter_3) == -1) {
                        // 将终结符加入当前符列表中，注意查重
                        if (!count(currents.begin(), currents.end(), *iter_3))
                            currents.push_back(*iter_3);
                        // 将非首位终结符加入栈顶符列表中，注意查重
                        if (iter_3 != iter_2->begin() && !count(stack_tops.begin(), stack_tops.end(), *iter_3))
                            stack_tops.push_back(*iter_3);
                    }
                }
            }
        }
        // 为LL1分析表开辟内存空间
        table = new Analyze_table_item* [stack_tops.size()];
        for (unsigned int i = 0; i < stack_tops.size(); i ++) {
            table[i] = new Analyze_table_item [currents.size()];
        }
        initialize_table();
    }
}
LL1::~LL1() {}

// 检查文法是否为最左推导文法
bool LL1::is_available() {
    for (Grammar::Production_iter iter_1 = G.G.begin(); iter_1 != G.G.end(); iter_1 ++) {
        if ((iter_1->second).size() > 1) {
            set<string> s;
            for (Grammar::Right_list_iter iter_2 = (iter_1->second).begin(); iter_2 != (iter_1->second).end(); iter_2 ++) {
                if (iter_2->front() == iter_1->first) { // 是否存在左递归
                    cout << "Left Recursion" << endl;
                    return false;
                }
                set<string> tmp = G.select_set_of(iter_1->first, *iter_2);
                int old_size = s.size();
                s.insert(tmp.begin(), tmp.end());
                if (s.size() < old_size + tmp.size()) { // 选择集合是否相交
                    cout << "Intersactive Select-set" << endl;
                    return false;
                }
            }
        } else if ((iter_1->second)[0][0] == iter_1->first) { // 是否存在左递归
            cout << "Left Recursion" << endl;
            return false;
        }
    }
    return true;
}

// 获取LL1分析表某项的指针，按行列表头：栈顶符和当前符查找
LL1::Analyze_table_item* LL1::get_op(string stack_top, string current) {
    int m = -1, n = -1;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        if (stack_tops[i] == stack_top) {
            m = i;
            break;
        }
    }
    for (unsigned int i = 0; i < currents.size(); i ++) {
        if (currents[i] == current) {
            n = i;
            break;
        }
    }
    // 将行列表头关键字转换为数组下标
    if (m != -1 && n != -1) return &table[m][n];
    else return NULL; // 如果没找到该元素则返回空
}

// 设置LL1表中某项的值，stack_op read_op 分别是对栈的操作和对输入流的操作
bool LL1::set_op(string stack_top, string current, vector<string> stack_op, char read_op) {
    Analyze_table_item* p = get_op(stack_top, current);
    if (!p) return false;
    p->stack_op = stack_op;
    p->read_op = read_op;
    return true;
}

// 输出LL1表以供调试
void LL1::print_table() {
    // 这类语句是用来更改控制台文字前景色和背景色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
    cout << setw(14) << " ";
    for (unsigned int i = 0; i < currents.size(); i ++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
        cout << setw(14) << currents[i];
    }
    cout << endl;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
        cout << setw(14) << stack_tops[i];
        for (unsigned int j = 0; j < currents.size(); j ++) {
            if (!((j&1)^(i&1))) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLACK);
            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_ON_WHITE);
            if (!table[i][j].stack_op.empty()) {
                cout << setw(10) << vector_join(" ", table[i][j].stack_op.begin(), table[i][j].stack_op.end());
                cout << ", " << setw(2) << table[i][j].read_op;
            } else cout << setw(14) << " ";
        }
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_ON_WHITE);
}

// 初始化LL1分析表
void LL1::initialize_table() {
    set_op("#", "#", str_split("OK", " "), '\0');
    for (vector<string>::iterator iter = stack_tops.begin() + 1; iter != stack_tops.end(); iter++) {
        if (G.symbol_type(*iter) != 1) {
            if ((*iter).find("qua") == 0) { // 如果是翻译文法对应符号
                for (auto cur_it: currents) { // 将翻译文法对应符号的一行全填为（epsilon，P）
                    set_op(*iter, cur_it, str_split("epsilon", " "), 'P');
                }
            } else { // 其他终结符的处理
                set_op(*iter, *iter, str_split("epsilon", " "), 'N');
            }
        } else { // 如果当前遍历到的符号为非终结符
            Grammar::Production_iter p = G.G.find(*iter);
            for (Grammar::Right_list_iter it = (p->second).begin(); it != (p->second).end(); it ++) {
                switch (G.symbol_type((*it)[0])) { // 判断右部每个产生式的开头符号
                    case 0: // epsilon
                        {
                            set<string> select = G.select_set_of(*iter, *it);
                            for (set<string>::iterator set_it = select.begin(); set_it != select.end(); set_it ++) {
                                set_op(*iter, *set_it, str_split("epsilon", " "), 'P');
                            }
                            break;
                        }
                    case 1: // VN
                        {
                            set<string> select = G.select_set_of(*iter, *it);
                            Grammar::Right_symbols rs = *it;
                            reverse(rs.begin(), rs.end());
                            for (set<string>::iterator set_it = select.begin(); set_it != select.end(); set_it ++) {
                                set_op(*iter, *set_it, rs, 'P');
                            }
                            break;
                        }
                    case-1: // VT
                        {
                            Grammar::Right_symbols rs;
                            if (it->size() > 1) {
                                rs.insert(rs.begin(), it->begin() + 1, it->end());
                                reverse(rs.begin(), rs.end());
                            } else {
                                rs.push_back("epsilon");
                            }
                            set_op(*iter, (*it)[0], rs, 'N');
                        }
                }
            }
        }
    }
}

// 语法分析主控函数
vector<Quarternary> LL1::check_and_translate(vector <Token> tokens) {
    vector<Quarternary> Qs; // 返回四元式序列
    vector<Quarternary> error; // 返回空表作为错误值
    vector<double*> operands; // 操作数栈，存四元式中的指针
    vector<string> syn; // 分析栈
    // 初始化 SYN=(#S) tokens=(..., #)
    syn.push_back("#");
    syn.push_back(G.S);
    Token t = {'#', 0};
    tokens.push_back(t);
    Token token = tokens.front();
    string w;
    while (1) {
        switch (token.kind) { // 将当前Token转为符合文法的字符串
            case 'K': w = G.KT[token.index]; break;
            case 'P': w = G.PT[token.index]; break;
            case 'I': w = "/I"; break;
            case 'C': w = "/C"; break;
            case 'c': w = G.cT[token.index]; break;
            case 'S': w = G.ST[token.index]; break;
            case '#': w = "#"; break;
        }
        Analyze_table_item* p = get_op(syn.back(), w); // 查LL1分析表
        if (!p || (p->stack_op).empty()) { // 如果查表越界或查到的表项为空则报错
            cout << "Syntax Error(?): wrong experession" << endl;
            return error;
        } else if (p->stack_op[0] == "OK") { // 如果查到OK则接收字符串返回四元式序列
            return Qs;
        } else {
            syn.pop_back();
            if (p->stack_op[0] != "epsilon") // 如果栈操作不为弹栈，则将栈操作压栈
                syn.insert(syn.end(), (p->stack_op).begin(), (p->stack_op).end());
            if (syn.back().length() >= 4 && syn.back().find("qua") == 0) { // 如果栈顶符为翻译文法符号
                char operat = syn.back()[3];
                if (operat == 'p') { // 对于quap将操作数保存入栈
                    operands.push_back(&G.CT[token.index]);
                } else if (operat == '.') { // 对于qua. 处理符号运算
                    double* res_1 = new double(0);
                    double* res_2 = operands.back();
                    double* res = new double;
                    operands.pop_back();
                    Quarternary Q = {'-', res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                } else { // 对于其他二元运算
                    double* res_2 = operands.back();
                    operands.pop_back();
                    double* res_1 = operands.back();
                    operands.pop_back();
                    double* res = new double;
                    Quarternary Q = {operat, res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                }
            }
            if (p->read_op == 'N') { // 如果当前输入流操作为N，则读下一Token
                if (tokens.empty()) return error;
                tokens.erase(tokens.begin());
                token = tokens.front();
            }
        }
    }
}
