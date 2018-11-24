#include "direct_generate.h"

Direct_Gen::Direct_Gen(Grammar& G): G(G) {
    int m = 7;

    vector<string> op = {".", "^", "+", "-", "*", "/", "#"};
    operators.insert(operators.begin(), op.begin(), op.end());

    int pm[7][7] = {
    //   .   ^   +   -   *   /   #
      {  1, -1,  1,  1,  1,  1, -1}, // .
      { -1,  1,  1,  1,  1,  1, -1}, // ^
      { -1, -1,  1,  1, -1, -1, -1}, // +
      { -1, -1,  1,  1, -1, -1, -1}, // -
      { -1, -1,  1,  1,  1,  1, -1}, // *
      { -1, -1,  1,  1,  1,  1, -1}, // /
      {  1,  1,  1,  1,  1,  1,  0}  // #
    };
    prior_matrix = new int* [m];
    for (int i = 0; i < m; i ++) {
        prior_matrix[i] = new int [m];
        for (int j = 0; j < m; j ++) {
            prior_matrix[i][j] = pm[i][j];
        }
    }

    sharp = 0;
    for (unsigned int i = 0; i <= G.PT.size(); i ++) {
        if (!G.PT[i].compare("#")) {
            sharp = i;
            break;
        }
    }
}
Direct_Gen::~Direct_Gen() {
    for (int i = 0; i < int(sizeof(prior_matrix) / sizeof(prior_matrix[0])); i++)
        delete [] prior_matrix[i];
    delete [] prior_matrix;
}
int Direct_Gen::operator_index(string x) {
    for (unsigned int i = 0; i < operators.size(); i ++) {
        if (x == operators[i]) return i;
    }
    return -1;
}

void Direct_Gen::trim_minus_sign(vector<Token>& tokens) {
    int substitute = -1;
    for (unsigned int i = 0; i < G.PT.size(); i ++) {
        if (G.PT[i] == ".") {
            substitute = i;
            break;
        }
    }
    if (substitute == -1) return;
    for (unsigned int i = 0; i < tokens.size(); i ++) {
        if (tokens[i].kind == 'P' && G.PT[tokens[i].index] == "-") {
            if (i == 0 || (tokens[i - 1].kind == 'P' && operator_index(G.PT[tokens[i - 1].index]) != -1)) {
                tokens[i].index = substitute;
            }
        }
    }
}

double* Direct_Gen::generate_quarternary(vector<Token>& tokens, vector<Quarternary>& Qs) {
    while (tokens[0].kind == 'P' && tokens[tokens.size() - 1].kind == 'P' &&
        G.PT[tokens[0].index] == "(" && G.PT[tokens[tokens.size() - 1].index] == ")") {
        tokens.pop_back();
        tokens.erase(tokens.begin());
    }
    if (tokens.size() == 1) {
        double *val = new double;
        *val = G.CT[tokens[0].index];
        return val;
    }
    Token token = {'P', sharp};
    tokens.insert(tokens.begin(), token); tokens.push_back(token);
    int bracket = 0;
    vector<Token>::iterator i, op;
    for (i = op = tokens.begin(); i != tokens.end(); i++) {
        if (i->kind != 'P') continue;
        if (G.PT[i->index] == "(") bracket --;
        else if (G.PT[i->index] == ")") bracket ++;
        else if (bracket == 0 && prior_matrix[operator_index(G.PT[op->index])][operator_index(G.PT[i->index])] > 0) op = i;
    }
    vector<Token> token_r(op + 1, tokens.end() - 1);
    double* res = new double;
    double* res_2 = generate_quarternary(token_r, Qs);
    if (G.PT[op->index] == ".") {
        double* zero = new double(0);
        Quarternary Q = {'-', zero, res_2, res};
        Qs.push_back(Q);
        return res;
    }
    vector<Token> token_l(tokens.begin() + 1, op);
    double* res_1 = generate_quarternary(token_l, Qs);
    Quarternary Q = {G.PT[op->index][0], res_1, res_2, res};
    Qs.push_back(Q);
    return res;
}

vector<Quarternary> Direct_Gen::translate(vector<Token> tokens) {
    vector<Quarternary> Qs;
    trim_minus_sign(tokens);
    if (tokens.size() > 1) generate_quarternary(tokens, Qs);
    else {
        double* res = new double;
        double* val = new double;
        *val = G.CT[tokens.back().index];
        Quarternary Q = {'#', val, NULL, res};
        Qs.push_back(Q);
    }
    return Qs;
}

double Direct_Gen::execute(vector<Quarternary> Qs) {
    print_qs(Qs);
    for (unsigned int i = 0; i < Qs.size(); i++) {
        switch(Qs[i].op) {
            case '^': *(Qs[i].res) = pow(*(Qs[i].a), *(Qs[i].b)); break;
            case '+': *(Qs[i].res) = *(Qs[i].a) + *(Qs[i].b); break;
            case '-': *(Qs[i].res) = *(Qs[i].a) - *(Qs[i].b); break;
            case '*': *(Qs[i].res) = *(Qs[i].a) * *(Qs[i].b); break;
            case '/': *(Qs[i].res) = *(Qs[i].a) / *(Qs[i].b); break;
            case '#': *(Qs[i].res) = *(Qs[i].a); break;
        }
    }
    return *(Qs.back().res);
}
