#include "Grammar.h"

// ��string sep�ָ����ַ���������vector<string>
vector<string> str_split(string str, string sep) {
    vector<string> str_list;
    char *c_str = new char[str.length()];
    strcpy(c_str, str.c_str());
    char *p = strtok(c_str, sep.c_str()); // ʹ��C���Կ⺯��strtok�ִ�
    while (p) {
        str_list.push_back(string(p));
        p = strtok(NULL, sep.c_str());
    }
    delete c_str;
    return str_list;
}

// ��vector<string>��string sep���ӳ�string�����أ���Χ[s, e)
string vector_join(string sep, vector<string>::iterator s, vector<string>::iterator e) {
    if (s > e) return "";
    string str = *s;
    if (s == e) return str;
    for (vector<string>::iterator it = s + 1; it != e; it ++) {
        str = str + sep + (*it);
    }
    return str;
}

// �ķ�������Ĺ��캯��
Grammar::Grammar(
    vector<string>& KT,
    vector<string>& PT,
    vector<string>& IT,
    vector<char>& cT,
    vector<string>& ST,
    vector<double>& CT
):KT(KT), PT(PT), IT(IT), cT(cT), ST(ST), CT(CT) {} // ��ʼ�������������
Grammar::~Grammar() {}

// ����ķ�����ʽ
void Grammar::add_production(string left, string right) { // left��right�ֱ�Ϊ����ʽ�󲿺��Ҳ�
    Production_iter iter = G.find(left);
    Right_symbols right_one = str_split(right, " ");
    if (iter == G.end()) {
        Right_lists right_list = {right_one};
        G.insert(pair<string, Right_lists>(left, right_list));
    } else {
        (iter->second).push_back(right_one);
    }
}

// ���ÿ�ʼ����
void Grammar::set_start(string start) {
    S = start;
}

// �жϷ������ͣ��ս�������ս�������ַ�
int Grammar::symbol_type(string x) {
    if (x == "epsilon") return 0; // ���ַ�
    else if (G.find(x) == G.end()) return -1; // һ���ս��
    else return 1; // ���ս��
}

// ��x���׷���
set<string> Grammar::first_set_of(Right_symbols x) {
    set<string> first_set;
    Right_lists right_lists;
    if (x.size() == 1) { // �����ѯ��x�ǵ�������
        if (symbol_type(x[0]) != 1) {
            first_set.insert(x[0]);
            return first_set;
        }
        map<string, set<string>>::iterator it = firsts.find(x[0]);
        if (it != firsts.end()) return it->second; // �����ǰ���ŵ�first���Ѿ�������������
        right_lists = (*G.find(x[0])).second;
    } else {
        right_lists.push_back(x);
    }
    // ������ѯ���Ŵ����Բ�ѯ����Ϊ�󲿵Ĳ���ʽ���Ҳ���ÿһ������
    for (Right_list_iter iter = right_lists.begin(); iter != right_lists.end(); iter ++) {
        string f = (*iter)[0];
        switch (symbol_type(f)) {
            case 0: // ���ַ�
            case-1: // �ս��
                first_set.insert(f); break;
            case 1: // ���ս��
                Right_symbol_iter iter_1;
                for (iter_1 = iter->begin(); iter_1 != iter->end(); iter_1 ++) {
                    set<string> tmp = first_set_of(str_split(*iter_1, " "));
                    if (tmp.count("epsilon")) { // first���Ƿ��п��ַ�
                        tmp.erase("epsilon");
                        first_set.insert(tmp.begin(), tmp.end());
                    } else {
                        first_set.insert(tmp.begin(), tmp.end());
                        break; // �������п��ַ���������
                    }
                }
                if (iter_1 == iter->end()) first_set.insert("epsilon"); // ����Ҳ����з��Ŷ����Ƴ��գ��򽫿��ַ�����first��
        }
    }
    if (x.size() == 1) firsts.insert(pair<string, set<string>>(x[0], first_set)); // ����ѯ��������Ѳ�ѯ�ֵ���
    return first_set;
}

// ��x�ĺ�̼�
set<string> Grammar::follow_set_of(string x) {
    map<string, set<string>>::iterator it = follows.find(x);
    if (it != follows.end()) return it->second; // ���x��follow���Ѿ������ֱ�Ӳ����
    set<string> follow_set;
    if (symbol_type(x) != 1) return follow_set; // ���x��Ϊ���ս�������Ϸ������ؿռ���
    if (x == S) follow_set.insert("#"); // ����ǿ�ʼ������Ҫ���Ǽ���#

    // �����ֵ��е�ÿһ���ķ�����ʽ
    Production_iter row;
    for (row = G.begin(); row != G.end(); row ++) {

        // ����ÿһ���ķ�����ʽ�е��Ӳ���ʽ
        for (Right_list_iter item = (row->second).begin(); item != (row->second).end(); item ++) {

            // �����Ӳ���ʽ���Ҳ���ÿһ������
            Right_symbol_iter where_x_is;
            for (where_x_is = item->begin(); where_x_is != item->end(); where_x_is ++) {
                if (*where_x_is == x) break;
            }
            if (where_x_is == item->end()) continue;
            // ����ҵ�һ���Ҳ�����x�Ĳ���ʽ �� x֮���з���
            if (where_x_is != item->end() - 1) {
                Right_symbols right_of_x(where_x_is + 1, item->end());
                set<string> first = first_set_of(right_of_x); // ����x֮��ķ��Ŵ���first����
                if (first.count("epsilon")) { // ���first�����п��ַ�
                    first.erase("epsilon");
                    if (row->first != x) { // ��һ���ܹؼ�������󲿺�x��ͬ�������ݹ飬�������ѭ��
                        set<string> follow = follow_set_of(row->first);
                        follow_set.insert(follow.begin(), follow.end());
                    }
                }
                follow_set.insert(first.begin(), first.end());
            } else { // ���x֮���޷���
                if (row->first != x) {
                    set<string> follow = follow_set_of(row->first);
                    follow_set.insert(follow.begin(), follow.end());
                }
            }
        }
    }
    follows.insert(pair<string, set<string>>(x, follow_set)); // �������������ֵ��Թ��´ο��ٲ鿴
    return follow_set;
}

// �����ʽleft -> right��ѡ��
set<string> Grammar::select_set_of(string left, Right_symbols right) {
    set<string> first = first_set_of(right);
    if (first.count("epsilon")) {
        first.erase("epsilon");
        set<string> follow = follow_set_of(left);
        first.insert(follow.begin(), follow.end());
    }
    return first;
}

// �����ķ������
ostream & operator<<(ostream &out, Grammar& grammar) {
    for (Grammar::Production_iter iter_1 = grammar.G.begin(); iter_1 != grammar.G.end(); iter_1++) {
        out << iter_1->first << " -> ";
        for (Grammar::Right_list_iter iter_2 = iter_1->second.begin(); iter_2 != iter_1->second.end(); iter_2 ++) {
            if (iter_2 != iter_1->second.begin()) out << "| ";
            cout << vector_join(" ", iter_2->begin(), iter_2->end());
        }
        cout << endl;
    }
    return out;
}
