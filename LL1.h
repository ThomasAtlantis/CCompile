#ifndef LL1_H_INCLUDED
#define LL1_H_INCLUDED

#include "Grammar.h"

class LL1 {
public:
    typedef struct {
        vector<string> stack_op;
        char read_op;
    } Analyze_table_item;

    Grammar& G;
    vector<string> currents;
    vector<string> stack_tops;
    Analyze_table_item** table;
    bool available;

    LL1(Grammar& grammar);
    ~LL1();
    bool is_available();
    Analyze_table_item* get_op(string stack_top, string current);
    bool set_op(string stack_top, string current, vector<string> stack_op, char read_op);
    void print_table();
    void initialize_table();
    bool check(vector <Token> tokens);
};

#endif // LL1_H_INCLUDED
