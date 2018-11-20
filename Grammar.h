#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED

#include "public_tools.h"

class Grammar {
public:
    // �������Ͷ���
    typedef map<string, vector<vector<string>>> Productions;
    typedef map<string, vector<vector<string>>>::iterator Production_iter;
    typedef vector<vector<string>> Right_lists;
    typedef vector<vector<string>>::iterator Right_list_iter;
    typedef vector<string> Right_symbols;
    typedef vector<string>::iterator Right_symbol_iter;

    // ��ĳ�Ա����
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

    // ���캯������������
    Grammar(
        vector<string>& KT,
        vector<string>& PT,
        vector<string>& IT,
        vector<char>& cT,
        vector<string>& ST,
        vector<double>& CT
    );
    ~Grammar();

    // ����ķ�
    void add_production(string left, string right);

    // ���ÿ�ʼ����
    void set_start(string start);

    // �����������
    friend ostream & operator<<(ostream &out, Grammar& grammar);

    // �жϷ������ͣ��ս�������ս�������ַ�
    int symbol_type(string x);

    // ��x���׷���
    set<string> first_set_of(Right_symbols x);

    // ��x�ĺ�̼�
    set<string> follow_set_of(string x);

    // �����ʽleft -> right��ѡ��
    set<string> select_set_of(string left, Right_symbols right);
};

// �����ķ������
ostream & operator<<(ostream &out, Grammar& grammar);
vector<string> str_split(string str, string sep);
string vector_join(string sep, vector<string>::iterator s, vector<string>::iterator e);

#endif // GRAMMAR_H_INCLUDED
