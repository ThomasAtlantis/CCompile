#include "quaternary.h"
#include "scanner.h"
#include "grammar.h"
#include "LL1.h"
#include "direct_generate.h"
#include "recursive_subprograms.h"
#include "quaternary.h"
#include "LR1.h"

void compile(string file_name) {
    // 将源程序全文一次加载到缓冲区中
    ifstream source_file(file_name.c_str());
    if (!source_file.is_open()) cout << "File Error: failed to open source file" << endl;
    string buffer((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());

    // 定义所有符号表
    vector <string> KT = {  // 预留关键字表
        "char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union",
        "unsigned", "void", "for", "do", "while", "break", "continue", "if", "else", "goto",
        "switch", "case", "default ", "return", "auto", "extern", "register", "static", "const",
        "sizeof", "typedef", "volatile"
    };
    vector <string> PT = {  // 界符表
        "#", "+", "-", "*", "/", "=", ">", "<", ",", ".", "&", "|", "!", "^", ";", "{", "}", "[", "]", ":",
        "?", "(", ")", "%",     //  0 - 23 单字符
        "+=", "-=", "*=", "/=", "%=", "<=", ">=", "==", "&=", "|=", "!=", "^=", "^|", "&&", "||", "<<",
        ">>", "//", "/*", "*/", // 24 - 43 两字符
        "<<=", ">>="            // 44 - 45 三字符
    }; // "#"必须在第一位，这是为了后面判断方便，以后改进
    vector <string> IT; // 标识符表
    vector <char  > cT; // 字符表
    vector <string> ST; // 字符串表
    vector <double> CT; // 常数表
    vector <Token> tokens; // 存储所有的token
    unsigned int cur_index = 0, line_label = 1; // 当前处理位置和当前行标
    ScannerGet sg; // 暂存扫描器返回值
    while (cur_index < buffer.length()) { // 扫描整个缓冲区
        sg = scanner(buffer, cur_index, line_label, KT, PT, IT, cT, ST, CT); // 获得当前错误信息和token
        if (sg.error_type == -1) { // 如果有错误
            cout << file_name << "(" << line_label << "): " << sg.error_log << endl; return; // 输出错误位置和具体信息
        } else if (sg.error_type == 0) { // 如果无错误
            tokens.push_back(sg.token); // 存储token
            //cout << "(" << sg.token.kind << ", " << sg.token.index << ")" << endl; // 输出token
        }
        // 其他情况为跳过注释或遇到EOF，只需要continue
    }
    /* 输出所有表的内容
    if (sg.error_type != -1) {
        cout << "# 关键字表     KT #" << endl;
        print_vector(KT);
        cout << "# 界符表       PT #" << endl;
        print_vector(PT);
        cout << "# 标识符表     IT #" << endl;
        print_vector(IT);
        cout << "# 字符常量表   cT #" << endl;
        print_vector(cT);
        cout << "# 字符串常量表 ST #" << endl;
        print_vector(ST);
        cout << "# 数字常量表   CT #" << endl;
        print_vector(CT);
    }
    //*/
    //* 初始化文法，添加文法产生式
    Grammar G(KT, PT, IT, cT, ST, CT);
    G.set_start("E");
    G.add_production("E", "F T1");
    G.add_production("T1", "+ F qua+ T1");
    G.add_production("T1", "- F qua- T1");
    G.add_production("T1", "epsilon");
    G.add_production("F", "H F1");
    G.add_production("F1", "* H qua* F1");
    G.add_production("F1", "/ H qua/ F1");
    G.add_production("F1", "epsilon");
    G.add_production("H", "H1");
    G.add_production("H", "- H1 qua.");
    G.add_production("H1", "K1 J");
    G.add_production("J", "^ K1 qua^");
    G.add_production("J", "epsilon");
    G.add_production("K2", "- K1 qua.");
    G.add_production("K2", "K1");
    G.add_production("K1", "/I quap");
    G.add_production("K1", "/C quap");
    G.add_production("K1", "( E )");
    //*/
    /*
    G.add_production("H", "H1 J");
    G.add_production("J", "^ H1 qua^");
    G.add_production("J", "epsilon");
    G.add_production("H1", "- K qua.");
    G.add_production("H1", "K");
    G.add_production("K", "/I quap");
    G.add_production("K", "/C quap");
    G.add_production("K", "( E )");
    */
    /*
    cout << G << endl;
    for (Grammar::Production_iter it_1 = G.G.begin(); it_1 != G.G.end(); it_1 ++) {
        for (Grammar::Right_list_iter it_2 = (it_1->second).begin(); it_2 != (it_1->second).end(); it_2 ++) {
            set<string> s = G.select_set_of(it_1->first, *it_2);
            cout << it_1->first << " -> " << vector_join(" ", it_2->begin(), it_2->end()) << " : ";
            for (set<string>::iterator iter = s.begin(); iter != s.end(); iter ++) {
                cout << (*iter) << " ";
            }
            cout << endl;
        }
    }
    */
    /* LL1语法分析方法
    LL1 ll1(G);
    if (ll1.available) {
        vector<Quarternary> Qs = ll1.check_and_translate(tokens);
        if (!Qs.empty()) {
            Direct_Gen d(G);
            cout << "Result of Expression: " << d.execute(Qs) << endl;
        }
    }
    //*/

    //* 递归子程序语法分析方法
    Recursub rec(G);
    if (rec.available) {
        vector<Quarternary> Qs = rec.check_and_translate(tokens);
        if (!Qs.empty()) {
            Direct_Gen d(G);
            cout << "Result of Expression: " << d.execute(Qs) << endl;
        }
    }
    //*/
    /*
    LR1 lr1(G);
    if (lr1.available && lr1.check(tokens)) {
        Direct_Gen d(G);
        cout << "Result of Expression: " << d.execute(d.translate(tokens)) << endl;
    }
    //*/
}
int main() { // compile函数总测试
    compile("test.src");
    return 0;
}
