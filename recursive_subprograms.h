#ifndef RECURSIVE_SUBPROGRAMS_H_INCLUDED
#define RECURSIVE_SUBPROGRAMS_H_INCLUDED

#include "public_tools.h"
#include "Grammar.h"
#include "scanner.h"
#include "quaternary.h"

class Recursub {
private:
    string token2str(Token token); // 将Token转化为文法可识别的字符串
public:
    Grammar& G; // 文法的引用
    bool available; // 文法是否可用，即是否为可最左推导文法

    Recursub(Grammar& G);
    ~Recursub();
    bool is_available(); // 文法是否可用，即是否为可最左推导文法，与LL1相同
    vector<Quarternary> check_and_translate(vector<Token> tokens); // 语法分析主控函数
    bool subprogram(
        vector<Token>& tokens, // token序列
        string left, // 当前子程序对应的非终结符
        vector<Quarternary>& Qs,  // 四元式序列
        vector<double*>& operands,  // 操作数栈
        vector<int>& layers // 当前递归层数
    ); // 递归子程序
};

#endif // RECURSIVE_SUBPROGRAMS_H_INCLUDED
