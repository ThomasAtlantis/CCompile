#include "scanner.h"

// 在向量的某个范围内查找key
template <typename T>
int vector_find(vector<T> vec, T key, unsigned int index_l, unsigned int index_r) {
    for (unsigned int i = index_l; i <= index_r; i ++) {
        if (vec[i] == key) return i; // 找到则返回位置
    }
    return -1; // 没找到则返回-1
}

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
    ) {
    ScannerGet result; // 返回结果

    // 跳过所有连续的空白符，到达下一单词的开头
    char current_char = buffer[cur_index ++];
    while (is_blank(current_char) && cur_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[cur_index ++];
    }

    // 记录单词开头位置，以便后文输出单词源码
    unsigned int old_index = cur_index - 1;

    // 判断单词的开始字符
    if (cur_index > buffer.length()) { // 如果扫描到文件尾，结束扫描
        result.error_type = 1; // 错误类型为EOF
    } else if (current_char == '\'') { // 识别字符，填cT表
        char ch;
        // 使用字符识别自动机判断
        if (process_character(buffer, cur_index, ch)) { // 识别
            cT.push_back(ch);
            result.error_type = 0;
            result.token.kind = 'c';
            result.token.index = cT.size() - 1;
        } else { // 不识别
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character";
        }
    } else if (current_char == '\"') { // 识别字符串，填ST表
        string str;
        // 使用字符串识别自动机判断
        if (process_character_string(buffer, cur_index, str)) { // 识别
            ST.push_back(str);
            result.error_type = 0;
            result.token.kind = 'S';
            result.token.index = ST.size() - 1;
        } else { // 不识别
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character string";
        }
    } else if (is_num(current_char)) { // 识别算术常数，填CT表
        double num;
        cur_index --; // 这里--与进入常数处理机的初始条件有关
        // 使用算术常数处理机判断
        if (process_constant_num(buffer, cur_index, num)) { // 识别
            CT.push_back(num);
            result.error_type = 0;
            result.token.kind = 'C';
            result.token.index = CT.size() - 1;
        } else { // 不识别
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect constant";
        }
    } else if (is_letter(current_char) or current_char == '_') { // 识别用户定义标识符和关键字，查KT表，查填IT表
        // 记录单词开始位置
        int l_index = cur_index - 1;

        // 超前扫描到单词结尾
        do current_char = buffer[cur_index ++];
        while (is_letter(current_char) or is_num(current_char) or current_char == '_');

        // 获得单词源码串
        string tmp = buffer.substr(l_index, cur_index - l_index - 1);

        // 查KT表
        int index = vector_find(KT, tmp, 0, KT.size() - 1);
        if (index != -1) { // 查到
            result.error_type = 0;
            result.token.kind = 'K';
            result.token.index = index;
        } else { // 没查到
            if (!IT.empty())
            index = vector_find(IT, tmp, 0, IT.size() - 1); // 查IT表
            result.error_type = 0;
            result.token.kind = 'I';
            if (IT.empty() || index == -1) { // 没查到就填表
                IT.push_back(tmp);
                result.token.index = IT.size() - 1;
            } else { // 查到就记录位置
                result.token.index = index;
            }
        }
        cur_index --; // 调整当前位置指针
    } else { // 识别界符，查PT表
        int index, indexes[3][2] = {{0, 23}, {24, 43}, {44, 45}};
        result.error_type = -1; result.token.kind = 'P';
        result.error_log = "Symbol Error: incorrect delimiters";

        // 分别查找当前位置开始的三个、两个、一个字符是否在PT表中，使用长度优先原则
        for (int i = 0; i < 3; i ++) {
            if (cur_index + i > buffer.length()) break;
            string key = buffer.substr(cur_index - 1, i + 1);
            index = vector_find(PT, key, indexes[i][0], indexes[i][1]);
            if (index != -1) {
                result.error_type = 0;
                result.token.index = index;
            } else break;
        }

        if (!result.error_type) { // 如果查到，即单词为合法结符
            if (!PT[result.token.index].compare("//")) { // 如果结符为//
                while(cur_index < buffer.length() && buffer[cur_index ++] != '\n'); // 调整当前指针，忽略到行尾
                line_label ++;
                result.error_type = 1; // 返回类型为注释，继续扫
                return result;
            } else if (!PT[result.token.index].compare("/*")) { // 如果界符为/*
                while(cur_index < buffer.length() - 1 && buffer.substr(cur_index ++, 2).compare("*/")) {
                    if (buffer[cur_index] == '\n') line_label ++;
                } // 修改当前指针位置到下一个*/之后
                cur_index += 1;
                if (cur_index >= buffer.length()) { // 如果扫到文件结束都没匹配*/则报错
                    result.error_type = -1;
                    result.error_log = "Symbol Error: incorrect annotation";
                } else result.error_type = 1; // 返回类型为注释，继续扫
                return result;
            }
        }
    }
//    if (result.error_type == 0) // 输出当前单词源码
//        cout << left << "[" << setw(4) << line_label << "] " << buffer.substr(old_index, cur_index - old_index) << " ";
    return result; // 返回
}
