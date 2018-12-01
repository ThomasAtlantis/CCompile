//#include "LR1.h"
//
//LR1::LR1(Grammar& G): G(G) {}
//
//LR1::~LR1() {}
//
//set<string> calc_first(Item item) {
//    vector<string> query_symbols;
//    set<string> new_expects;
//    if (item.symbols.size() > 1)
//        query_symbols.insert(query_symbols.end(), item.symbols.begin() + 1, item.symbols.end());
//    for (auto it: item.expects) {
//        query_symbols.push_back(it);
//        set<string> tmp = G.first_set_of(query_symbols);
//        new_expects.insert(all(tmp));
//        query_symbols.pop_back();
//    }
//    return new_expects;
//}
//
//void LR1::initialize_table() {
//    Item item = {{G.S}, {"#"}, 0};
//    vector<Item> close = {item};
//    vector<vector<string>> symbolss;
//    for (auto it_1: close) {
//        auto expand = G.G.find(it_1.symbols[0]);
//        if (expand != G.G.end()) {
//            for (auto it_2: expand->second) {
//                if (!symbolss.count())
//                close.emplace_back(Item {, calc_first(it_1), 0});
//            }
//        }
//    }
//}
//
//void LR1::go() {
//
//}
//
//bool LR1::check(vector<Token> tokens) {
//    return true;
//}
