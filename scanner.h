#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "public_tools.h"
#include "process_constant_num.h"
#include "process_character.h"
#include "process_character_string.h"

// ɨ������������ֵ����
typedef struct {
    Token token;      // token
    int error_type;   // �������ͣ�0���޴���1��EOF��-1��fatal error
    string error_log; // �������ϸ����
} ScannerGet;

// ɨ��������
ScannerGet scanner(
    string& buffer,     // �洢Դ����ȫ�ĵĻ�����
    unsigned int& cur_index,  // ��ǰ����λ��
    unsigned int& line_label, // ��ǰ�����б�
    vector<string>& KT, // �ؼ��ֱ�
    vector<string>& PT, // �����
    vector<string>& IT, // ��ʶ����
    vector<char>& cT,   // �ַ�������
    vector<string>& ST, // �ַ���������
    vector<double>& CT  // ����������
);

template <typename T>
void print_vector(vector<T>& vec) {
    if (vec.empty()) return;
    for (unsigned int i = 0; i < vec.size(); i++) {
        cout << left << "[" << setw(4) << i << "] " << setw(30) << vec[i];
        if (i % 4 == 3) cout << endl;
    }
    if (vec.size() % 4) cout << endl;
}

#endif // SCANNER_H_INCLUDED
