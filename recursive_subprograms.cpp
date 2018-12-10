#include "recursive_subprograms.h"

// �ݹ��ӳ�����Ĺ��캯��
Recursub::Recursub(Grammar& G): G(G) {
    available = is_available(); // ���ݹ��ӳ���Ե�ǰ�����Ƿ�����
}

Recursub::~Recursub() {}

// ���ݹ��ӳ���Ե�ǰ�����Ƿ����ã���LL1::is_available()��ȫ��ͬ
bool Recursub::is_available() {
    for (Grammar::Production_iter iter_1 = G.G.begin(); iter_1 != G.G.end(); iter_1 ++) {
        if ((iter_1->second).size() > 1) {
            set<string> s;
            for (Grammar::Right_list_iter iter_2 = (iter_1->second).begin(); iter_2 != (iter_1->second).end(); iter_2 ++) {
                if (iter_2->front() == iter_1->first) {
                    cout << "Left Recursion" << endl;
                    return false;
                }
                set<string> tmp = G.select_set_of(iter_1->first, *iter_2);
                int old_size = s.size();
                s.insert(tmp.begin(), tmp.end());
                if (s.size() < old_size + tmp.size()) {
                    cout << "Intersactive Select-set" << endl;
                    return false;
                }
            }
        } else if ((iter_1->second)[0][0] == iter_1->first) {
            cout << "Left Recursion" << endl;
            return false;
        }
    }
    return true;
}

// ��tokenת��Ϊ�ķ���ʶ����ַ���
string Recursub::token2str(Token token) {
    string w;
    switch (token.kind) {
        case 'K': w = G.KT[token.index]; break;
        case 'P': w = G.PT[token.index]; break;
        case 'I': w = "/I"; break; // /I����ת���ַ���ƥ��IT��������
        case 'C': w = "/C"; break; // /C����ת���ַ���ƥ��CT��������
        case 'c': w = G.cT[token.index]; break;
        case 'S': w = G.ST[token.index]; break;
        case '#': w = "#"; break;
    }
    return w;
}

// ����Ӧ�����ķ��ĵݹ��ӳ���
bool Recursub::subprogram(
        vector<Token>& tokens, // token����
        string left, // ��ǰ�ӳ����Ӧ�ķ��ս��
        vector<Quarternary>& Qs,  // ��Ԫʽ����
        vector<double*>& operands,  // ������ջ
        vector<int>& layers // ��ǰ�ݹ����
    ) {
    // ���е�PRINT GRAMMAR TREE �� END~�������Ĳ��ֶ������������﷨�������
    /// PRINT GRAMMAR TREE
    cout << "        ";
    for (int i = 0; i < layers.size() - 1; i ++) if (layers[i]) cout << "   "; else cout << "��  ";
    if (layers.back()) cout << "���� "; else cout << "���� "; cout << left << endl;
    /// END PRINT GRAMMAR TREE

    bool epsilon_flag = false; // �Ƿ����Ƴ�epsilon
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        if (rl->size() == 1 && rl->front() == "epsilon") epsilon_flag = true;
    }

    bool unfit = true; // �Ƿ�����ķ���׼ȷ��˵����������Ƿ��ڵ�ǰ���ս����select����

    Grammar::Production_iter p = G.G.find(left);
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        // ���ڵ�ǰ���ս��Ϊ�󲿵����в���ʽ������select��
        set<string> select = G.select_set_of(p->first, *rl);
        string w = token2str(tokens.front());
        if (!select.count(w)) continue; // �����ǰselect��������������ţ�������������ʽ
        unfit = false; // �����ǰ���ſ��Ƶ�������unfit��Ϊ��
        Token token_sav;
        for (Grammar::Right_symbol_iter rs = rl->begin(); rs != rl->end(); rs ++) { // �������в���ʽ�Ҳ����з���
            if (G.symbol_type(*rs) == 1) { // ���Ϊ���ս��
                /// PRINT GRAMMAR TREE
                if (rs != rl->end() - 1) layers.push_back(0); else layers.push_back(1);
                /// END PRINT GRAMMAR TREE

                // ���˷��ս����Ϊ�ӳ����ǩ���µݹ�
                if (!subprogram(tokens, *rs, Qs, operands, layers)) return false;

                /// PRINT GRAMMAR TREE
                layers.pop_back();
                /// END PRINT GRAMMAR TREE
                w = token2str(tokens.front());
            } else {
                /// PRINT GRAMMAR TREE
                if ((*rs).find("qua") == string::npos && (*rs) != "epsilon") cout << setw(8) << *rs; else cout << "        ";
                for (int i = 0; i < layers.size(); i ++) if (layers[i]) cout << "   "; else cout << "��  ";
                if (rs == rl->end() - 1) cout << "���� "; else cout << "���� "; cout << *rs << endl;
                /// END PRINT GRAMMAR TREE

                if ((*rs).find("qua") == 0) { // �����ǰ����Ϊ�����ķ�����
                    char operat = (*rs)[3];
                    if (operat == 'p') { // quap������������ջ
                        operands.push_back(&G.CT[token_sav.index]);
                    } else if (operat == '.') { // qua.�����������
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
                    continue; // ֱ�ӱ�����һ������
                }
                if (w == *rs) { // ������ս��ƥ��
                    token_sav = tokens.front();
                    tokens.erase(tokens.begin());
                } else if (!epsilon_flag) return false; // ��������ֲ����Ƴ����򱨴�
            }
        }
        break;
    }
    return !unfit;
}

// �﷨�������غ���
vector<Quarternary> Recursub::check_and_translate(vector<Token> tokens) {
    Token token = {'#', 0};
    tokens.push_back(token); // ������token���к����#
    vector<int> layers = {1}; // ��ʼ���ݹ�����б�
    vector<Quarternary> Qs; // ��Ԫʽ����
    vector<Quarternary> error; // �����ؿ�����
    vector<double*> operands; // ������ջ
    if (subprogram(tokens, G.S, Qs, operands, layers)) { // ���õݹ��ӳ��򣬴ӿ�ʼ����ǵ��ӳ���ʼ
        return Qs;
    } else { // ����
        cout << "Syntax Error(?): wrong experession" << endl;
        return error;
    }
}
