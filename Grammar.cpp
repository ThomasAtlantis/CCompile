#include "Grammar.h"
// string 用sep分割子字符串，返回vector<string>类型列表
vector<string> str_split(string str, string sep) {
    vector<string> str_list;
    char *c_str = new char[str.length()];
    strcpy(c_str, str.c_str());
    char *p = strtok(c_str, sep.c_str());
    while (p) {
        str_list.push_back(string(p));
        p = strtok(NULL, sep.c_str());
    }
    delete c_str;
    return str_list;
}

string vector_join(string sep, vector<string>::iterator s, vector<string>::iterator e) {
    if (s > e) return "";
    string str = *s;
    if (s == e) return str;
    for (vector<string>::iterator it = s + 1; it != e; it ++) {
        str = str + sep + (*it);
    }
    return str;
}

Grammar::Grammar(
    vector<string>& KT,
    vector<string>& PT,
    vector<string>& IT,
    vector<char>& cT,
    vector<string>& ST,
    vector<double>& CT
):KT(KT), PT(PT), IT(IT), cT(cT), ST(ST), CT(CT) {}
Grammar::~Grammar() {}

// 添加文法
void Grammar::add_production(string left, string right) {
    Production_iter iter = G.find(left);
    Right_symbols right_one = str_split(right, " ");
    if (iter == G.end()) {
        Right_lists right_list = {right_one};
        G.insert(pair<string, Right_lists>(left, right_list));
    } else {
        (iter->second).push_back(right_one);
    }
}

// 设置开始符号
void Grammar::set_start(string start) {
    S = start;
}

// 判断符号类型：终结符、非终结符、空字符
int Grammar::symbol_type(string x) {
    if (x == "epsilon") return 0;
    else if (G.find(x) == G.end()) return -1;
    else return 1;
}

// 求x的首符集
set<string> Grammar::first_set_of(Right_symbols x) {
    set<string> first_set;
    Right_lists right_lists;
    if (x.size() == 1) {
        if (symbol_type(x[0]) != 1) {
            first_set.insert(x[0]);
            return first_set;
        }
        map<string, set<string>>::iterator it = firsts.find(x[0]);
        if (it != firsts.end()) return it->second;
        right_lists = (*G.find(x[0])).second;
    } else {
        right_lists.push_back(x);
    }
    for (Right_list_iter iter = right_lists.begin(); iter != right_lists.end(); iter ++) {
        string f = (*iter)[0];
        switch (symbol_type(f)) {
            case 0: // Epsilon
            case-1: // VT
                first_set.insert(f); break;
            case 1: // VN
                Right_symbol_iter iter_1;
                for (iter_1 = iter->begin(); iter_1 != iter->end(); iter_1 ++) {
                    set<string> tmp = first_set_of(str_split(*iter_1, " "));
                    if (tmp.count("epsilon")) {
                        tmp.erase("epsilon");
                        first_set.insert(tmp.begin(), tmp.end());
                    } else {
                        first_set.insert(tmp.begin(), tmp.end());
                        break;
                    }
                }
                if (iter_1 == iter->end()) first_set.insert("epsilon");
        }
    }
    if (x.size() == 1) firsts.insert(pair<string, set<string>>(x[0], first_set));
    return first_set;
}

// 求x的后继集
set<string> Grammar::follow_set_of(string x) {
    map<string, set<string>>::iterator it = follows.find(x);
    if (it != follows.end()) return it->second;
    set<string> follow_set;
    if (symbol_type(x) != 1) {
        return follow_set;
    }
    if (x == S) {
        follow_set.insert("#");
    }
    // 遍历字典中的每一行文法产生式
    Production_iter row;
    for (row = G.begin(); row != G.end(); row ++) {
        // 遍历每一行文法产生式中的子产生式
        for (Right_list_iter item = (row->second).begin(); item != (row->second).end(); item ++) {
            // 遍历子产生式的右部的每一个符号
            Right_symbol_iter where_x_is;
            for (where_x_is = item->begin(); where_x_is != item->end(); where_x_is ++) {
                if (*where_x_is == x) break;
            }
            if (where_x_is == item->end()) continue;
            if (where_x_is != item->end() - 1) {
                Right_symbols right_of_x(where_x_is + 1, item->end());
                set<string> first = first_set_of(right_of_x);
                if (first.count("epsilon")) {
                    first.erase("epsilon");
                    if (row->first != x) {
                        set<string> follow = follow_set_of(row->first);
                        follow_set.insert(follow.begin(), follow.end());
                    }
                }
                follow_set.insert(first.begin(), first.end());
            } else {
                if (row->first != x) {
                    set<string> follow = follow_set_of(row->first);
                    follow_set.insert(follow.begin(), follow.end());
                }
            }
        }
    }
    follows.insert(pair<string, set<string>>(x, follow_set));
    return follow_set;
}

// 求产生式left -> right的选择集
set<string> Grammar::select_set_of(string left, Right_symbols right) {
    set<string> first = first_set_of(right);
    if (first.count("epsilon")) {
        first.erase("epsilon");
        set<string> follow = follow_set_of(left);
        first.insert(follow.begin(), follow.end());
    }
    return first;
}

// 重载文法的输出
ostream & operator<<(ostream &out, Grammar& grammar) {
    for (Grammar::Production_iter iter_1 = grammar.G.begin(); iter_1 != grammar.G.end(); iter_1++) {
        out << iter_1->first << " -> ";
        for (Grammar::Right_list_iter iter_2 = iter_1->second.begin(); iter_2 != iter_1->second.end(); iter_2 ++) {
            if (iter_2 != iter_1->second.begin()) out << "| ";
            cout << vector_join(" ", iter_2->begin(), iter_2->end());
        }
        cout << endl;
    }
    return out;
}
