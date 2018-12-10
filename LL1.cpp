#include "LL1.h"

// LL1���캯������ʼ����Ա����
LL1::LL1(Grammar& grammar): G(grammar) {
    // ע���ڴ˳����б��뱣֤��#���ڵ�һλ
    available = false;
    if (is_available()) { // �����ķ��ĺϷ���
        available = true;

        // ����ջ�������б�stack_tops�͵�ǰ�����б�currents
        currents.push_back("#");
        stack_tops.push_back("#");
        for (Grammar::Production_iter iter_1 = grammar.G.begin(); iter_1 != grammar.G.end(); iter_1++) {
            stack_tops.push_back(iter_1->first); // �����ս������ջ�����б�
            for (Grammar::Right_list_iter iter_2 = iter_1->second.begin(); iter_2 != iter_1->second.end(); iter_2 ++) {
                for (Grammar::Right_symbol_iter iter_3 = iter_2->begin(); iter_3 != iter_2->end(); iter_3 ++) {
                    if (grammar.symbol_type(*iter_3) == -1) {
                        // ���ս�����뵱ǰ���б��У�ע�����
                        if (!count(currents.begin(), currents.end(), *iter_3))
                            currents.push_back(*iter_3);
                        // ������λ�ս������ջ�����б��У�ע�����
                        if (iter_3 != iter_2->begin() && !count(stack_tops.begin(), stack_tops.end(), *iter_3))
                            stack_tops.push_back(*iter_3);
                    }
                }
            }
        }
        // ΪLL1���������ڴ�ռ�
        table = new Analyze_table_item* [stack_tops.size()];
        for (unsigned int i = 0; i < stack_tops.size(); i ++) {
            table[i] = new Analyze_table_item [currents.size()];
        }
        initialize_table();
    }
}
LL1::~LL1() {}

// ����ķ��Ƿ�Ϊ�����Ƶ��ķ�
bool LL1::is_available() {
    for (Grammar::Production_iter iter_1 = G.G.begin(); iter_1 != G.G.end(); iter_1 ++) {
        if ((iter_1->second).size() > 1) {
            set<string> s;
            for (Grammar::Right_list_iter iter_2 = (iter_1->second).begin(); iter_2 != (iter_1->second).end(); iter_2 ++) {
                if (iter_2->front() == iter_1->first) { // �Ƿ������ݹ�
                    cout << "Left Recursion" << endl;
                    return false;
                }
                set<string> tmp = G.select_set_of(iter_1->first, *iter_2);
                int old_size = s.size();
                s.insert(tmp.begin(), tmp.end());
                if (s.size() < old_size + tmp.size()) { // ѡ�񼯺��Ƿ��ཻ
                    cout << "Intersactive Select-set" << endl;
                    return false;
                }
            }
        } else if ((iter_1->second)[0][0] == iter_1->first) { // �Ƿ������ݹ�
            cout << "Left Recursion" << endl;
            return false;
        }
    }
    return true;
}

// ��ȡLL1������ĳ���ָ�룬�����б�ͷ��ջ�����͵�ǰ������
LL1::Analyze_table_item* LL1::get_op(string stack_top, string current) {
    int m = -1, n = -1;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        if (stack_tops[i] == stack_top) {
            m = i;
            break;
        }
    }
    for (unsigned int i = 0; i < currents.size(); i ++) {
        if (currents[i] == current) {
            n = i;
            break;
        }
    }
    // �����б�ͷ�ؼ���ת��Ϊ�����±�
    if (m != -1 && n != -1) return &table[m][n];
    else return NULL; // ���û�ҵ���Ԫ���򷵻ؿ�
}

// ����LL1����ĳ���ֵ��stack_op read_op �ֱ��Ƕ�ջ�Ĳ����Ͷ��������Ĳ���
bool LL1::set_op(string stack_top, string current, vector<string> stack_op, char read_op) {
    Analyze_table_item* p = get_op(stack_top, current);
    if (!p) return false;
    p->stack_op = stack_op;
    p->read_op = read_op;
    return true;
}

// ���LL1���Թ�����
void LL1::print_table() {
    // ����������������Ŀ���̨����ǰ��ɫ�ͱ���ɫ
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
    cout << setw(14) << " ";
    for (unsigned int i = 0; i < currents.size(); i ++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
        cout << setw(14) << currents[i];
    }
    cout << endl;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLUE);
        cout << setw(14) << stack_tops[i];
        for (unsigned int j = 0; j < currents.size(); j ++) {
            if (!((j&1)^(i&1))) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLACK);
            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_ON_WHITE);
            if (!table[i][j].stack_op.empty()) {
                cout << setw(10) << vector_join(" ", table[i][j].stack_op.begin(), table[i][j].stack_op.end());
                cout << ", " << setw(2) << table[i][j].read_op;
            } else cout << setw(14) << " ";
        }
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_ON_WHITE);
}

// ��ʼ��LL1������
void LL1::initialize_table() {
    set_op("#", "#", str_split("OK", " "), '\0');
    for (vector<string>::iterator iter = stack_tops.begin() + 1; iter != stack_tops.end(); iter++) {
        if (G.symbol_type(*iter) != 1) {
            if ((*iter).find("qua") == 0) { // ����Ƿ����ķ���Ӧ����
                for (auto cur_it: currents) { // �������ķ���Ӧ���ŵ�һ��ȫ��Ϊ��epsilon��P��
                    set_op(*iter, cur_it, str_split("epsilon", " "), 'P');
                }
            } else { // �����ս���Ĵ���
                set_op(*iter, *iter, str_split("epsilon", " "), 'N');
            }
        } else { // �����ǰ�������ķ���Ϊ���ս��
            Grammar::Production_iter p = G.G.find(*iter);
            for (Grammar::Right_list_iter it = (p->second).begin(); it != (p->second).end(); it ++) {
                switch (G.symbol_type((*it)[0])) { // �ж��Ҳ�ÿ������ʽ�Ŀ�ͷ����
                    case 0: // epsilon
                        {
                            set<string> select = G.select_set_of(*iter, *it);
                            for (set<string>::iterator set_it = select.begin(); set_it != select.end(); set_it ++) {
                                set_op(*iter, *set_it, str_split("epsilon", " "), 'P');
                            }
                            break;
                        }
                    case 1: // VN
                        {
                            set<string> select = G.select_set_of(*iter, *it);
                            Grammar::Right_symbols rs = *it;
                            reverse(rs.begin(), rs.end());
                            for (set<string>::iterator set_it = select.begin(); set_it != select.end(); set_it ++) {
                                set_op(*iter, *set_it, rs, 'P');
                            }
                            break;
                        }
                    case-1: // VT
                        {
                            Grammar::Right_symbols rs;
                            if (it->size() > 1) {
                                rs.insert(rs.begin(), it->begin() + 1, it->end());
                                reverse(rs.begin(), rs.end());
                            } else {
                                rs.push_back("epsilon");
                            }
                            set_op(*iter, (*it)[0], rs, 'N');
                        }
                }
            }
        }
    }
}

// �﷨�������غ���
vector<Quarternary> LL1::check_and_translate(vector <Token> tokens) {
    vector<Quarternary> Qs; // ������Ԫʽ����
    vector<Quarternary> error; // ���ؿձ���Ϊ����ֵ
    vector<double*> operands; // ������ջ������Ԫʽ�е�ָ��
    vector<string> syn; // ����ջ
    // ��ʼ�� SYN=(#S) tokens=(..., #)
    syn.push_back("#");
    syn.push_back(G.S);
    Token t = {'#', 0};
    tokens.push_back(t);
    Token token = tokens.front();
    string w;
    while (1) {
        switch (token.kind) { // ����ǰTokenתΪ�����ķ����ַ���
            case 'K': w = G.KT[token.index]; break;
            case 'P': w = G.PT[token.index]; break;
            case 'I': w = "/I"; break;
            case 'C': w = "/C"; break;
            case 'c': w = G.cT[token.index]; break;
            case 'S': w = G.ST[token.index]; break;
            case '#': w = "#"; break;
        }
        Analyze_table_item* p = get_op(syn.back(), w); // ��LL1������
        if (!p || (p->stack_op).empty()) { // ������Խ���鵽�ı���Ϊ���򱨴�
            cout << "Syntax Error(?): wrong experession" << endl;
            return error;
        } else if (p->stack_op[0] == "OK") { // ����鵽OK������ַ���������Ԫʽ����
            return Qs;
        } else {
            syn.pop_back();
            if (p->stack_op[0] != "epsilon") // ���ջ������Ϊ��ջ����ջ����ѹջ
                syn.insert(syn.end(), (p->stack_op).begin(), (p->stack_op).end());
            if (syn.back().length() >= 4 && syn.back().find("qua") == 0) { // ���ջ����Ϊ�����ķ�����
                char operat = syn.back()[3];
                if (operat == 'p') { // ����quap��������������ջ
                    operands.push_back(&G.CT[token.index]);
                } else if (operat == '.') { // ����qua. �����������
                    double* res_1 = new double(0);
                    double* res_2 = operands.back();
                    double* res = new double;
                    operands.pop_back();
                    Quarternary Q = {'-', res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                } else { // ����������Ԫ����
                    double* res_2 = operands.back();
                    operands.pop_back();
                    double* res_1 = operands.back();
                    operands.pop_back();
                    double* res = new double;
                    Quarternary Q = {operat, res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                }
            }
            if (p->read_op == 'N') { // �����ǰ����������ΪN�������һToken
                if (tokens.empty()) return error;
                tokens.erase(tokens.begin());
                token = tokens.front();
            }
        }
    }
}
