#ifndef RECURSIVE_SUBPROGRAMS_H_INCLUDED
#define RECURSIVE_SUBPROGRAMS_H_INCLUDED

#include "public_tools.h"
#include "Grammar.h"
#include "scanner.h"
#include "quaternary.h"

class Recursub {
private:
    string token2str(Token token); // ��Tokenת��Ϊ�ķ���ʶ����ַ���
public:
    Grammar& G; // �ķ�������
    bool available; // �ķ��Ƿ���ã����Ƿ�Ϊ�������Ƶ��ķ�

    Recursub(Grammar& G);
    ~Recursub();
    bool is_available(); // �ķ��Ƿ���ã����Ƿ�Ϊ�������Ƶ��ķ�����LL1��ͬ
    vector<Quarternary> check_and_translate(vector<Token> tokens); // �﷨�������غ���
    bool subprogram(
        vector<Token>& tokens, // token����
        string left, // ��ǰ�ӳ����Ӧ�ķ��ս��
        vector<Quarternary>& Qs,  // ��Ԫʽ����
        vector<double*>& operands,  // ������ջ
        vector<int>& layers // ��ǰ�ݹ����
    ); // �ݹ��ӳ���
};

#endif // RECURSIVE_SUBPROGRAMS_H_INCLUDED
