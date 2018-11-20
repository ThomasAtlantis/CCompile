#include "LL1.h"

LL1::LL1(Grammar& grammar): G(grammar) {
    // 注意在此程序中必须保证“#”在第一位
    currents.push_back("#");
    stack_tops.push_back("#");
    for (Grammar::Production_iter iter_1 = grammar.G.begin(); iter_1 != grammar.G.end(); iter_1++) {
        stack_tops.push_back(iter_1->first);
        for (Grammar::Right_list_iter iter_2 = iter_1->second.begin(); iter_2 != iter_1->second.end(); iter_2 ++) {
            for (Grammar::Right_symbol_iter iter_3 = iter_2->begin(); iter_3 != iter_2->end(); iter_3 ++) {
                if (grammar.symbol_type(*iter_3) == -1) {
                    if (!count(currents.begin(), currents.end(), *iter_3))
                        currents.push_back(*iter_3);
                    if (iter_3 != iter_2->begin() && !count(stack_tops.begin(), stack_tops.end(), *iter_3))
                        stack_tops.push_back(*iter_3);
                }
            }
        }
    }
    table = new Analyze_table_item* [stack_tops.size()];
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        table[i] = new Analyze_table_item [currents.size()];
    }
    available = false;
    if (is_available()) {
        initialize_table();
        available = true;
    }
}
LL1::~LL1() {}

bool LL1::is_available() {
    ///TODO 检查是否为LL(1)文法
    return true;
}

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
    if (m != -1 && n != -1) return &table[m][n];
    else return NULL;
}

bool LL1::set_op(string stack_top, string current, vector<string> stack_op, char read_op) {
    Analyze_table_item* p = get_op(stack_top, current);
    if (!p) return false;
    p->stack_op = stack_op;
    p->read_op = read_op;
    return true;
}

    void LL1::print_table() {
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

void LL1::initialize_table() {
    set_op("#", "#", str_split("OK", " "), '\0');
    for (vector<string>::iterator iter = stack_tops.begin() + 1; iter != stack_tops.end(); iter++) {
        if (G.symbol_type(*iter) != 1) {
            set_op(*iter, *iter, str_split("epsilon", " "), 'N');
        } else {
            Grammar::Production_iter p = G.G.find(*iter);
            for (Grammar::Right_list_iter it = (p->second).begin(); it != (p->second).end(); it ++) {
                switch (G.symbol_type((*it)[0])) {
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

bool LL1::check(vector <Token> tokens) {
    vector<string> syn;
    syn.push_back("#");
    syn.push_back(G.S);
    Token t = {'P', 0};
    tokens.push_back(t);

//    for (unsigned int i = 0; i < syn.size(); i ++) {
//        cout << "[" << syn[i] << "] ";
//    }
//    cout << "\t";
//    for (unsigned int i = 0; i < tokens.size(); i ++) {
//        cout << "<" << tokens[i].kind << ", " << tokens[i].index << "> ";
//    }
//    cout << endl;

    Token token = tokens.front();
    tokens.erase(tokens.begin());
    string w;
    while (1) {
        switch (token.kind) {
            case 'K': w = G.KT[token.index]; break;
            case 'P': w = G.PT[token.index]; break;
            case 'I':
            case 'C': w = "I"; break;
            case 'c': w = G.cT[token.index]; break;
            case 'S': w = G.ST[token.index]; break;
        }
        Analyze_table_item* p = get_op(syn.back(), w);
        if (!p || (p->stack_op).empty()) {
            cout << "Syntax Error(?): wrong experession" << endl;
            return false;
        } else if (p->stack_op[0] == "OK") {
            return true;
        } else {
            syn.pop_back();
            if (p->stack_op[0] != "epsilon")
                syn.insert(syn.end(), (p->stack_op).begin(), (p->stack_op).end());
            if (p->read_op == 'N') {
                if (tokens.empty()) return false;
                token = tokens.front();
                tokens.erase(tokens.begin());
            }
//            for (unsigned int i = 0; i < syn.size(); i ++) {
//                cout << "[" << syn[i] << "] ";
//            }
//            cout << "\t";
//            for (unsigned int i = 0; i < tokens.size(); i ++) {
//                cout << "<" << tokens[i].kind << ", " << tokens[i].index << "> ";
//            }
//            cout << endl;
        }
    }
}
