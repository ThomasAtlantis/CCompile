#include "scanner.h"
#include "grammar.h"
#include "LL1.h"

void compile(string file_name) {
    ifstream source_file(file_name.c_str());
    if (!source_file.is_open()) cout << "File Error: failed to open source file" << endl;
    string buffer((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
    vector <string> KT = { // Ԥ���ؼ��ֱ�
        "char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union",
        "unsigned", "void", "for", "do", "while", "break", "continue", "if", "else", "goto",
        "switch", "case", "default ", "return", "auto", "extern", "register", "static", "const",
        "sizeof", "typedef", "volatile"
    };
    vector <string> PT = { // �����
        "#", "+", "-", "*", "/", "=", ">", "<", ",", ".", "&", "|", "!", ";", "{", "}", "[", "]", ":",
        "?", "(", ")", "%", //0 - 21 ���ַ�
        "+=", "-=", "*=", "/=", "%=", "<=", ">=", "==", "&=", "|=", "!=", "^=", "&&", "||", "<<",
        ">>", "//", "/*", "*/", // 22 - 40 ���ַ�
        "<<=", ">>=" // 41 - 42 ���ַ�
    }; // ע���ڴ˳����б��뱣֤��#���ڵ�һλ
    vector <string> IT; // ��ʶ����
    vector <char  > cT; // �ַ���
    vector <string> ST; // �ַ�����
    vector <double> CT; // ������
    vector <Token> tokens;
    unsigned int cur_index = 0, line_label = 1;
    while (cur_index < buffer.length()) {
        ScannerGet sg = scanner(buffer, cur_index, line_label, KT, PT, IT, cT, ST, CT);
        if (sg.error_type == -1) {
            cout << file_name << "(" << line_label << "): " << sg.error_log << endl; return;
        } else if (sg.error_type == 0) {
            tokens.push_back(sg.token);
        }
    }
//    if (sg.error_type != -1) {
//        cout << "# �ؼ��ֱ�     KT #" << endl;
//        print_vector(KT);
//        cout << "# �����       PT #" << endl;
//        print_vector(PT);
//        cout << "# ��ʶ����     IT #" << endl;
//        print_vector(IT);
//        cout << "# �ַ�������   cT #" << endl;
//        print_vector(cT);
//        cout << "# �ַ��������� ST #" << endl;
//        print_vector(ST);
//        cout << "# ���ֳ�����   CT #" << endl;
//        print_vector(CT);
//    }
    Grammar G(KT, PT, IT, cT, ST, CT);
    G.set_start("E");
    G.add_production("E", "T E1");
    G.add_production("E1", "+ T E1");
    G.add_production("E1", "- T E1");
    G.add_production("E1", "epsilon");
    G.add_production("T", "F T1");
    G.add_production("T1", "* F T1");
    G.add_production("T1", "/ F T1");
    G.add_production("T1", "epsilon");
    G.add_production("F", "I");
    G.add_production("F", "( E )");
//    cout << G << endl;
//    for (Grammar::Production_iter it_1 = G.G.begin(); it_1 != G.G.end(); it_1 ++) {
//        for (Grammar::Right_list_iter it_2 = (it_1->second).begin(); it_2 != (it_1->second).end(); it_2 ++) {
//            set<string> s = G.select_set_of(it_1->first, *it_2);
//            cout << it_1->first << " -> " << vector_join(" ", it_2->begin(), it_2->end()) << " : ";
//            for (set<string>::iterator iter = s.begin(); iter != s.end(); iter ++) {
//                cout << (*iter) << " ";
//            }
//            cout << endl;
//        }
//    }
    LL1 ll1(G);
    bool result = ll1.check(tokens);
    cout << (result ? "True" : "False") << endl;
}
int main() {
    compile("test.src");
    return 0;
}