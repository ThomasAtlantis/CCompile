#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED

#include "public_tools.h"

class Grammar {
public:
    // 类内类型定义
    typedef map<string, vector<vector<string>>> Productions;
    typedef map<string, vector<vector<string>>>::iterator Production_iter;
    typedef vector<vector<string>> Right_lists;
    typedef vector<vector<string>>::iterator Right_list_iter;
    typedef vector<string> Right_symbols;
    typedef vector<string>::iterator Right_symbol_iter;

    // 类的成员变量
    Productions G;
    string S;
    map<string, set<string>> firsts;
    map<string, set<string>> follows;
    vector<string>& KT;
    vector<string>& PT;
    vector<string>& IT;
    vector<char>& cT;
    vector<string>& ST;
    vector<double>& CT;

    // 构造函数和析构函数
    Grammar(
        vector<string>& KT,
        vector<string>& PT,
        vector<string>& IT,
        vector<char>& cT,
        vector<string>& ST,
        vector<double>& CT
    );
    ~Grammar();

    // 添加文法
    void add_production(string left, string right);

    // 设置开始符号
    void set_start(string start);

    // 重载输出函数
    friend ostream & operator<<(ostream &out, Grammar& grammar);

    // 判断符号类型：终结符、非终结符、空字符
    int symbol_type(string x);

    // 求x的首符集
    set<string> first_set_of(Right_symbols x);

    // 求x的后继集
    set<string> follow_set_of(string x);

    // 求产生式left -> right的选择集
    set<string> select_set_of(string left, Right_symbols right);
};

// 重载文法的输出
ostream & operator<<(ostream &out, Grammar& grammar);
vector<string> str_split(string str, string sep);
string vector_join(string sep, vector<string>::iterator s, vector<string>::iterator e);

#endif // GRAMMAR_H_INCLUDED
