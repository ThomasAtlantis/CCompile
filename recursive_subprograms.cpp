#include "recursive_subprograms.h"

// 递归子程序类的构造函数
Recursub::Recursub(Grammar& G): G(G) {
    available = is_available(); // 检查递归子程序对当前程序是否适用
}

Recursub::~Recursub() {}

// 检查递归子程序对当前程序是否适用，与LL1::is_available()完全相同
bool Recursub::is_available() {
    for (Grammar::Production_iter iter_1 = G.G.begin(); iter_1 != G.G.end(); iter_1 ++) {
        if ((iter_1->second).size() > 1) {
            set<string> s;
            for (Grammar::Right_list_iter iter_2 = (iter_1->second).begin(); iter_2 != (iter_1->second).end(); iter_2 ++) {
                if (iter_2->front() == iter_1->first) {
                    cout << "Left Recursion" << endl;
                    return false;
                }
                set<string> tmp = G.select_set_of(iter_1->first, *iter_2);
                int old_size = s.size();
                s.insert(tmp.begin(), tmp.end());
                if (s.size() < old_size + tmp.size()) {
                    cout << "Intersactive Select-set" << endl;
                    return false;
                }
            }
        } else if ((iter_1->second)[0][0] == iter_1->first) {
            cout << "Left Recursion" << endl;
            return false;
        }
    }
    return true;
}

// 将token转换为文法可识别的字符串
string Recursub::token2str(Token token) {
    string w;
    switch (token.kind) {
        case 'K': w = G.KT[token.index]; break;
        case 'P': w = G.PT[token.index]; break;
        case 'I': w = "/I"; break; // /I代表转移字符，匹配IT表所有项
        case 'C': w = "/C"; break; // /C代表转移字符，匹配CT表所有项
        case 'c': w = G.cT[token.index]; break;
        case 'S': w = G.ST[token.index]; break;
        case '#': w = "#"; break;
    }
    return w;
}

// 自适应输入文法的递归子程序
bool Recursub::subprogram(
        vector<Token>& tokens, // token序列
        string left, // 当前子程序对应的非终结符
        vector<Quarternary>& Qs,  // 四元式序列
        vector<double*>& operands,  // 操作数栈
        vector<int>& layers // 当前递归层数
    ) {
    // 所有的PRINT GRAMMAR TREE 和 END~括起来的部分都是用来生成语法树的输出
    /// PRINT GRAMMAR TREE
    cout << "        ";
    for (int i = 0; i < layers.size() - 1; i ++) if (layers[i]) cout << "   "; else cout << "│  ";
    if (layers.back()) cout << "└─ "; else cout << "├─ "; cout << left << endl;
    /// END PRINT GRAMMAR TREE

    bool epsilon_flag = false; // 是否能推出epsilon
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        if (rl->size() == 1 && rl->front() == "epsilon") epsilon_flag = true;
    }

    bool unfit = true; // 是否符合文法，准确的说是输入符号是否在当前非终结符的select集中

    Grammar::Production_iter p = G.G.find(left);
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        // 对于当前非终结符为左部的所有产生式，计算select集
        set<string> select = G.select_set_of(p->first, *rl);
        string w = token2str(tokens.front());
        if (!select.count(w)) continue; // 如果当前select集不包含输入符号，遍历其他产生式
        unfit = false; // 如果当前符号可推导到，将unfit设为假
        Token token_sav;
        for (Grammar::Right_symbol_iter rs = rl->begin(); rs != rl->end(); rs ++) { // 遍历可行产生式右部所有符号
            if (G.symbol_type(*rs) == 1) { // 如果为非终结符
                /// PRINT GRAMMAR TREE
                if (rs != rl->end() - 1) layers.push_back(0); else layers.push_back(1);
                /// END PRINT GRAMMAR TREE

                // 将此非终结符作为子程序标签向下递归
                if (!subprogram(tokens, *rs, Qs, operands, layers)) return false;

                /// PRINT GRAMMAR TREE
                layers.pop_back();
                /// END PRINT GRAMMAR TREE
                w = token2str(tokens.front());
            } else {
                /// PRINT GRAMMAR TREE
                if ((*rs).find("qua") == string::npos && (*rs) != "epsilon") cout << setw(8) << *rs; else cout << "        ";
                for (int i = 0; i < layers.size(); i ++) if (layers[i]) cout << "   "; else cout << "│  ";
                if (rs == rl->end() - 1) cout << "└─ "; else cout << "├─ "; cout << *rs << endl;
                /// END PRINT GRAMMAR TREE

                if ((*rs).find("qua") == 0) { // 如果当前符号为翻译文法符号
                    char operat = (*rs)[3];
                    if (operat == 'p') { // quap，将操作数入栈
                        operands.push_back(&G.CT[token_sav.index]);
                    } else if (operat == '.') { // qua.处理符号运算
                        double* res_1 = new double(0);
                        double* res_2 = operands.back();
                        double* res = new double;
                        operands.pop_back();
                        Quarternary Q = {'-', res_1, res_2, res};
                        Qs.push_back(Q);
                        operands.push_back(res);
                    } else { // 处理其他二元运算
                        double* res_2 = operands.back();
                        operands.pop_back();
                        double* res_1 = operands.back();
                        operands.pop_back();
                        double* res = new double;
                        Quarternary Q = {operat, res_1, res_2, res};
                        Qs.push_back(Q);
                        operands.push_back(res);
                    }
                    continue; // 直接遍历下一个符号
                }
                if (w == *rs) { // 如果非终结符匹配
                    token_sav = tokens.front();
                    tokens.erase(tokens.begin());
                } else if (!epsilon_flag) return false; // 否则，如果又不能推出空则报错
            }
        }
        break;
    }
    return !unfit;
}

// 语法分析主控函数
vector<Quarternary> Recursub::check_and_translate(vector<Token> tokens) {
    Token token = {'#', 0};
    tokens.push_back(token); // 在输入token序列后加入#
    vector<int> layers = {1}; // 初始化递归层数列表
    vector<Quarternary> Qs; // 四元式序列
    vector<Quarternary> error; // 报错返回空序列
    vector<double*> operands; // 操作数栈
    if (subprogram(tokens, G.S, Qs, operands, layers)) { // 调用递归子程序，从开始符标记的子程序开始
        return Qs;
    } else { // 报错
        cout << "Syntax Error(?): wrong experession" << endl;
        return error;
    }
}
