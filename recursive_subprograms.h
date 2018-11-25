#ifndef RECURSIVE_SUBPROGRAMS_H_INCLUDED
#define RECURSIVE_SUBPROGRAMS_H_INCLUDED

#include "public_tools.h"
#include "Grammar.h"
#include "scanner.h"

class Recursub {
private:
    string token2str(Token token);
public:
    Grammar& G;
    bool available;

    Recursub(Grammar& G);
    ~Recursub();
    bool is_available();
    bool check(vector<Token> tokens);
    bool subprogram(vector<Token>& tokens, string left, vector<int>& layers);
};

#endif // RECURSIVE_SUBPROGRAMS_H_INCLUDED
