#include "scanner.h"

bool string_equals(string a, string b) {
    return a.compare(b) == 0;
}

template <typename T>
int vector_find(
    vector<T> vec,
    T key,
    bool (* equals)(T, T),
    unsigned int index_l,
    unsigned int index_r
    ) {
    for (unsigned int i = index_l; i <= index_r; i ++) {
        if (equals(vec[i], key)) return i;
    }
    return -1;
}

template <typename T>
void print_vector(vector<T> vec) {
    if (vec.empty()) return;
    for (unsigned int i = 0; i < vec.size(); i++) {
        cout << left << "[" << setw(4) << i << "] " << setw(30) << vec[i];
        if (i % 4 == 3) cout << endl;
    }
    if (vec.size() % 4) cout << endl;
}

ScannerGet scanner(
    string& buffer,
    unsigned int& cur_index,
    unsigned int& line_label,
    vector<string>& KT,
    vector<string>& PT,
    vector<string>& IT,
    vector<char>& cT,
    vector<string>& ST,
    vector<double>& CT
    ) {
    ScannerGet result;
    char current_char = buffer[cur_index ++];
    while (is_blank(current_char) && cur_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[cur_index ++];
    }
    //unsigned int old_index = cur_index - 1;
    if (cur_index > buffer.length()) { // 如果扫描到文件尾，结束扫描
        result.error_type = 1;
    } else if (current_char == '\'') { // 识别字符，填cT表
        char ch;
        if (process_character(buffer, cur_index, ch)) {
            cT.push_back(ch);
            result.error_type = 0;
            result.token.kind = 'c';
            result.token.index = cT.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character";
        }
    } else if (current_char == '\"') { // 识别字符串，填ST表
        string str;
        if (process_character_string(buffer, cur_index, str)) {
            ST.push_back(str);
            result.error_type = 0;
            result.token.kind = 'S';
            result.token.index = ST.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character string";
        }
    } else if (is_num(current_char)) { // 识别算术常数，填CT表
        double num;
        cur_index --;
        if (process_constant_num(buffer, cur_index, num)) {
            CT.push_back(num);
            result.error_type = 0;
            result.token.kind = 'C';
            result.token.index = CT.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect constant";
        }
    } else if (is_letter(current_char) or current_char == '_') { // 识别用户定义标识符和关键字，查KT表，查填IT表
        int l_index = cur_index - 1;
        do current_char = buffer[cur_index ++];
        while (is_letter(current_char) or is_num(current_char) or current_char == '_');
        string tmp = buffer.substr(l_index, cur_index - l_index - 1);
        int index = vector_find(KT, tmp, string_equals, 0, KT.size() - 1);
        if (index != -1) {
            result.error_type = 0;
            result.token.kind = 'K';
            result.token.index = index;
        } else {
            if (!IT.empty())
            index = vector_find(IT, tmp, string_equals, 0, IT.size() - 1);
            result.error_type = 0;
            result.token.kind = 'I';
            if (IT.empty() || index == -1) {
                IT.push_back(tmp);
                result.token.index = IT.size() - 1;
            } else {
                result.token.index = index;
            }
        }
        cur_index --;
    } else { // 识别界符，查PT表
        int index, indexes[3][2] = {{0, 21}, {22, 40}, {41, 42}};
        result.error_type = -1; result.token.kind = 'P';
        result.error_log = "Symbol Error: incorrect delimiters";
        for (int i = 0; i < 3; i ++) {
            if (cur_index + i > buffer.length()) break;
            string key = buffer.substr(cur_index - 1, i + 1);
            index = vector_find(PT, key, string_equals, indexes[i][0], indexes[i][1]);
            if (index != -1) {
                result.error_type = 0;
                result.token.index = index;
            } else break;
        }
        if (!result.error_type) {
            if (!PT[result.token.index].compare("//")) {
                while(cur_index < buffer.length() && buffer[cur_index ++] != '\n');
                line_label ++;
                result.error_type = 1;
                return result;
            } else if (!PT[result.token.index].compare("/*")) {
                while(cur_index < buffer.length() - 1 && buffer.substr(cur_index ++, 2).compare("*/")) {
                    if (buffer[cur_index] == '\n') line_label ++;
                }
                cur_index += 1;
                if (cur_index >= buffer.length()) {
                    result.error_type = -1;
                    result.error_log = "Symbol Error: incorrect annotation";
                } else result.error_type = 1;
                return result;
            }
        }
    }
//    if (result.error_type == 0)
//        cout << left << "[" << setw(4) << line_label << "] " << buffer.substr(old_index, cur_index - old_index) << " ";
    return result;
}
