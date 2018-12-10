#include "Grammar.h"

// 用string sep分割子字符串，返回vector<string>
vector<string> str_split(string str, string sep) {
    vector<string> str_list;
    char *c_str = new char[str.length()];
    strcpy(c_str, str.c_str());
    char *p = strtok(c_str, sep.c_str()); // 使用C语言库函数strtok分词
    while (p) {
        str_list.push_back(string(p));
        p = strtok(NULL, sep.c_str());
    }
    delete c_str;
    return str_list;
}

// 将vector<string>用string sep连接成string并返回，范围[s, e)
string vector_join(string sep, vector<string>::iterator s, vector<string>::iterator e) {
    if (s > e) return "";
    string str = *s;
    if (s == e) return str;
    for (vector<string>::iterator it = s + 1; it != e; it ++) {
        str = str + sep + (*it);
    }
    return str;
}

// 文法管理类的构造函数
Grammar::Grammar(
    vector<string>& KT,
    vector<string>& PT,
    vector<string>& IT,
    vector<char>& cT,
    vector<string>& ST,
    vector<double>& CT
):KT(KT), PT(PT), IT(IT), cT(cT), ST(ST), CT(CT) {} // 初始化各类表格的引用
Grammar::~Grammar() {}

// 添加文法产生式
void Grammar::add_production(string left, string right) { // left，right分别为产生式左部和右部
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
    if (x == "epsilon") return 0; // 空字符
    else if (G.find(x) == G.end()) return -1; // 一般终结符
    else return 1; // 非终结符
}

// 求x的首符集
set<string> Grammar::first_set_of(Right_symbols x) {
    set<string> first_set;
    Right_lists right_lists;
    if (x.size() == 1) { // 如果查询的x是单个符号
        if (symbol_type(x[0]) != 1) {
            first_set.insert(x[0]);
            return first_set;
        }
        map<string, set<string>>::iterator it = firsts.find(x[0]);
        if (it != firsts.end()) return it->second; // 如果当前符号的first集已经计算过，查表返回
        right_lists = (*G.find(x[0])).second;
    } else {
        right_lists.push_back(x);
    }
    // 遍历查询符号串或以查询符号为左部的产生式的右部的每一个符号
    for (Right_list_iter iter = right_lists.begin(); iter != right_lists.end(); iter ++) {
        string f = (*iter)[0];
        switch (symbol_type(f)) {
            case 0: // 空字符
            case-1: // 终结符
                first_set.insert(f); break;
            case 1: // 非终结符
                Right_symbol_iter iter_1;
                for (iter_1 = iter->begin(); iter_1 != iter->end(); iter_1 ++) {
                    set<string> tmp = first_set_of(str_split(*iter_1, " "));
                    if (tmp.count("epsilon")) { // first集是否含有空字符
                        tmp.erase("epsilon");
                        first_set.insert(tmp.begin(), tmp.end());
                    } else {
                        first_set.insert(tmp.begin(), tmp.end());
                        break; // 若不含有空字符则计算结束
                    }
                }
                if (iter_1 == iter->end()) first_set.insert("epsilon"); // 如果右部所有符号都能推出空，则将空字符加入first集
        }
    }
    if (x.size() == 1) firsts.insert(pair<string, set<string>>(x[0], first_set)); // 将查询结果填入已查询字典中
    return first_set;
}

// 求x的后继集
set<string> Grammar::follow_set_of(string x) {
    map<string, set<string>>::iterator it = follows.find(x);
    if (it != follows.end()) return it->second; // 如果x的follow集已经算过，直接查表返回
    set<string> follow_set;
    if (symbol_type(x) != 1) return follow_set; // 如果x不为非终结符，不合法，返回空集合
    if (x == S) follow_set.insert("#"); // 如果是开始符，不要忘记加入#

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
            // 如果找到一个右部含有x的产生式 且 x之后有符号
            if (where_x_is != item->end() - 1) {
                Right_symbols right_of_x(where_x_is + 1, item->end());
                set<string> first = first_set_of(right_of_x); // 计算x之后的符号串的first集合
                if (first.count("epsilon")) { // 如果first集含有空字符
                    first.erase("epsilon");
                    if (row->first != x) { // 这一步很关键，如果左部和x相同，结束递归，否则会死循环
                        set<string> follow = follow_set_of(row->first);
                        follow_set.insert(follow.begin(), follow.end());
                    }
                }
                follow_set.insert(first.begin(), first.end());
            } else { // 如果x之后无符号
                if (row->first != x) {
                    set<string> follow = follow_set_of(row->first);
                    follow_set.insert(follow.begin(), follow.end());
                }
            }
        }
    }
    follows.insert(pair<string, set<string>>(x, follow_set)); // 将计算结果存入字典以供下次快速查看
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
