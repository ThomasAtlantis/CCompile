#ifndef DIRECT_GENERATE_H_INCLUDED
#define DIRECT_GENERATE_H_INCLUDED

#include "public_tools.h"
#include "scanner.h"
#include "Grammar.h"
#include "quaternary.h"

#include "direct_generate.h"

class Direct_Gen {
private:
    Grammar& G;
    int** prior_matrix;
    int sharp; // "#" 在PT表中的位置
    vector<string> operators;

    int operator_index(string x);
    void trim_minus_sign(vector<Token>& tokens);
    double* generate_quarternary(vector<Token>& tokens, vector<Quarternary>& Qs);

public:
    Direct_Gen(Grammar& G);
    ~Direct_Gen();
    vector<Quarternary> translate(vector<Token> tokens);
    double execute(vector<Quarternary> Qs);
};

#endif // DIRECT_GENERATE_H_INCLUDED
