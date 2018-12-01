//#ifndef LR1_H_INCLUDED
//#define LR1_H_INCLUDED
//
//#include "public_tools.h"
//#include "Grammar.h"
//#include "scanner.h"
//#include "LL1.h"
//
//#define all(x) (x).begin(),(x).end()
//
//class LR1 {
//public:
//
//    typedef struct {
//        vector <string> symbols;
//        set <string> expects;
//        size_t dot;
//    } Item;
//
//    typedef struct {
//        char op;
//        size_t index;
//    } Table_ele;
//
//    typedef vector<map<string, Table_ele>> Analyze_table;
//
//    Grammar& G;
//    Analyze_table table;
//    bool available;
//
//    LR1(Grammar& G);
//    ~LR1();
//
//    void initialize_table();
//    void go();
//    bool check(vector<Token> tokens);
//};
//
//#endif // LR1_H_INCLUDED
