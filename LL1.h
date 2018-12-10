#ifndef LL1_H_INCLUDED
#define LL1_H_INCLUDED

#include "public_tools.h"
#include "Grammar.h"
#include "quaternary.h"

class LL1 {
public:
    typedef struct {
        vector<string> stack_op; // �Է���ջ�Ĳ������ķ����ŵ�����������ѹ��ջ��
        char read_op;            // ���������Ŀ��ƣ�'N'��ʾ����һ��token��'P'��ʾ����
    } Analyze_table_item;        // �������е�ÿһ��

    Grammar& G; // ���ķ�������
    vector<string> currents;     //��ǰ������
    vector<string> stack_tops;   //ջ��������
    Analyze_table_item** table;  //������
    bool available; //�ķ��Ƿ���ã����ķ��Ƿ����LL(1)�ķ�

    LL1(Grammar& grammar);
    ~LL1();
    bool is_available(); // �ж��ķ��Ƿ����LL1�ķ�
    Analyze_table_item* get_op(string stack_top, string current); // ��ȡLL1�������ĳһ���ָ��
    bool set_op(string stack_top, string current, vector<string> stack_op, char read_op); // ����LL1�������ĳһ���ֵ
    void print_table(); // ���LL1���������ڲ���
    void initialize_table(); //��ʼ��LL1������
    vector<Quarternary> check_and_translate(vector <Token> tokens); // �﷨��������
};

#endif // LL1_H_INCLUDED
