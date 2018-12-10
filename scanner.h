#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "public_tools.h"
#include "process_constant_num.h"
#include "process_character.h"
#include "process_character_string.h"

// 扫描器函数返回值类型
typedef struct {
    Token token;      // token
    int error_type;   // 错误类型：0：无错误；1：EOF；-1：fatal error
    string error_log; // 错误的详细报告
} ScannerGet;

// 扫描器函数
ScannerGet scanner(
    string& buffer,     // 存储源程序全文的缓冲区
    unsigned int& cur_index,  // 当前处理位置
    unsigned int& line_label, // 当前处理行标
    vector<string>& KT, // 关键字表
    vector<string>& PT, // 界符表
    vector<string>& IT, // 标识符表
    vector<char>& cT,   // 字符常量表
    vector<string>& ST, // 字符串常量表
    vector<double>& CT  // 算术常量表
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
