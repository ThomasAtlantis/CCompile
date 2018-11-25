#include "recursive_subprograms.h"

Recursub::Recursub(Grammar& G): G(G) {
    available = is_available();
}

Recursub::~Recursub() {}

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

string Recursub::token2str(Token token) {
    string w;
    switch (token.kind) {
        case 'K': w = G.KT[token.index]; break;
        case 'P': w = G.PT[token.index]; break;
        case 'I': w = "/I"; break;
        case 'C': w = "/C"; break;
        case 'c': w = G.cT[token.index]; break;
        case 'S': w = G.ST[token.index]; break;
        case '#': w = "#"; break;
    }
    return w;
}

bool Recursub::subprogram(vector<Token>& tokens, string left, vector<int>& layers) {
    /// PRINT GRAMMAR TREE
    cout << "   ";
    for (int i = 0; i < layers.size() - 1; i ++) if (layers[i]) cout << "   "; else cout << "©¦  ";
    if (layers.back()) cout << "©¸©¤ "; else cout << "©À©¤ "; cout << left << endl;
    /// END PRINT GRAMMAR TREE
    Grammar::Production_iter p = G.G.find(left);
    bool epsilon_flag = false;
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        if (rl->size() == 1 && rl->front() == "epsilon") epsilon_flag = true;
    }
    for (Grammar::Right_list_iter rl = (p->second).begin(); rl != (p->second).end(); rl ++) {
        set<string> select = G.select_set_of(p->first, *rl);
        string w = token2str(tokens.front());
        if (!select.count(w)) continue;
        for (Grammar::Right_symbol_iter rs = rl->begin(); rs != rl->end(); rs ++) {
            if (G.symbol_type(*rs) == 1) {
                /// PRINT GRAMMAR TREE
                if (rs != rl->end() - 1) layers.push_back(0); else layers.push_back(1);
                /// END PRINT GRAMMAR TREE
                if (!subprogram(tokens, *rs, layers)) return false;
                /// PRINT GRAMMAR TREE
                layers.pop_back();
                /// END PRINT GRAMMAR TREE
                w = token2str(tokens.front());
            } else {
                if (w == *rs) {
                    tokens.erase(tokens.begin());
                    /// PRINT GRAMMAR TREE
                    cout << setw(3) << *rs;
                    for (int i = 0; i < layers.size(); i ++) if (layers[i]) cout << "   "; else cout << "©¦  ";
                    if (rs == rl->end() - 1) cout << "©¸©¤ "; else cout << "©À©¤ "; cout << *rs << endl;
                    /// END PRINT GRAMMAR TREE
                } else if (!epsilon_flag) return false;
            }
        }
        break;
    }
    return true;
}


bool Recursub::check(vector<Token> tokens) {
    Token token = {'#', 0};
    tokens.push_back(token);
    vector<int> layers = {1};
    return subprogram(tokens, G.S, layers);
}
