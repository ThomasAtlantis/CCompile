#include "scanner.h"

// ��������ĳ����Χ�ڲ���key
template <typename T>
int vector_find(vector<T> vec, T key, unsigned int index_l, unsigned int index_r) {
    for (unsigned int i = index_l; i <= index_r; i ++) {
        if (vec[i] == key) return i; // �ҵ��򷵻�λ��
    }
    return -1; // û�ҵ��򷵻�-1
}

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
    ) {
    ScannerGet result; // ���ؽ��

    // �������������Ŀհ׷���������һ���ʵĿ�ͷ
    char current_char = buffer[cur_index ++];
    while (is_blank(current_char) && cur_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[cur_index ++];
    }

    // ��¼���ʿ�ͷλ�ã��Ա�����������Դ��
    unsigned int old_index = cur_index - 1;

    // �жϵ��ʵĿ�ʼ�ַ�
    if (cur_index > buffer.length()) { // ���ɨ�赽�ļ�β������ɨ��
        result.error_type = 1; // ��������ΪEOF
    } else if (current_char == '\'') { // ʶ���ַ�����cT��
        char ch;
        // ʹ���ַ�ʶ���Զ����ж�
        if (process_character(buffer, cur_index, ch)) { // ʶ��
            cT.push_back(ch);
            result.error_type = 0;
            result.token.kind = 'c';
            result.token.index = cT.size() - 1;
        } else { // ��ʶ��
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character";
        }
    } else if (current_char == '\"') { // ʶ���ַ�������ST��
        string str;
        // ʹ���ַ���ʶ���Զ����ж�
        if (process_character_string(buffer, cur_index, str)) { // ʶ��
            ST.push_back(str);
            result.error_type = 0;
            result.token.kind = 'S';
            result.token.index = ST.size() - 1;
        } else { // ��ʶ��
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character string";
        }
    } else if (is_num(current_char)) { // ʶ��������������CT��
        double num;
        cur_index --; // ����--����볣��������ĳ�ʼ�����й�
        // ʹ����������������ж�
        if (process_constant_num(buffer, cur_index, num)) { // ʶ��
            CT.push_back(num);
            result.error_type = 0;
            result.token.kind = 'C';
            result.token.index = CT.size() - 1;
        } else { // ��ʶ��
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect constant";
        }
    } else if (is_letter(current_char) or current_char == '_') { // ʶ���û������ʶ���͹ؼ��֣���KT������IT��
        // ��¼���ʿ�ʼλ��
        int l_index = cur_index - 1;

        // ��ǰɨ�赽���ʽ�β
        do current_char = buffer[cur_index ++];
        while (is_letter(current_char) or is_num(current_char) or current_char == '_');

        // ��õ���Դ�봮
        string tmp = buffer.substr(l_index, cur_index - l_index - 1);

        // ��KT��
        int index = vector_find(KT, tmp, 0, KT.size() - 1);
        if (index != -1) { // �鵽
            result.error_type = 0;
            result.token.kind = 'K';
            result.token.index = index;
        } else { // û�鵽
            if (!IT.empty())
            index = vector_find(IT, tmp, 0, IT.size() - 1); // ��IT��
            result.error_type = 0;
            result.token.kind = 'I';
            if (IT.empty() || index == -1) { // û�鵽�����
                IT.push_back(tmp);
                result.token.index = IT.size() - 1;
            } else { // �鵽�ͼ�¼λ��
                result.token.index = index;
            }
        }
        cur_index --; // ������ǰλ��ָ��
    } else { // ʶ��������PT��
        int index, indexes[3][2] = {{0, 23}, {24, 43}, {44, 45}};
        result.error_type = -1; result.token.kind = 'P';
        result.error_log = "Symbol Error: incorrect delimiters";

        // �ֱ���ҵ�ǰλ�ÿ�ʼ��������������һ���ַ��Ƿ���PT���У�ʹ�ó�������ԭ��
        for (int i = 0; i < 3; i ++) {
            if (cur_index + i > buffer.length()) break;
            string key = buffer.substr(cur_index - 1, i + 1);
            index = vector_find(PT, key, indexes[i][0], indexes[i][1]);
            if (index != -1) {
                result.error_type = 0;
                result.token.index = index;
            } else break;
        }

        if (!result.error_type) { // ����鵽��������Ϊ�Ϸ����
            if (!PT[result.token.index].compare("//")) { // ������Ϊ//
                while(cur_index < buffer.length() && buffer[cur_index ++] != '\n'); // ������ǰָ�룬���Ե���β
                line_label ++;
                result.error_type = 1; // ��������Ϊע�ͣ�����ɨ
                return result;
            } else if (!PT[result.token.index].compare("/*")) { // ������Ϊ/*
                while(cur_index < buffer.length() - 1 && buffer.substr(cur_index ++, 2).compare("*/")) {
                    if (buffer[cur_index] == '\n') line_label ++;
                } // �޸ĵ�ǰָ��λ�õ���һ��*/֮��
                cur_index += 1;
                if (cur_index >= buffer.length()) { // ���ɨ���ļ�������ûƥ��*/�򱨴�
                    result.error_type = -1;
                    result.error_log = "Symbol Error: incorrect annotation";
                } else result.error_type = 1; // ��������Ϊע�ͣ�����ɨ
                return result;
            }
        }
    }
//    if (result.error_type == 0) // �����ǰ����Դ��
//        cout << left << "[" << setw(4) << line_label << "] " << buffer.substr(old_index, cur_index - old_index) << " ";
    return result; // ����
}
