#include "public_tools.h"

bool string_equals(string a, string b) {
    return a.compare(b) == 0;
}

template <typename T>
int vector_find(
    vector<T> vec,
    T key,
    bool (* equals)(T, T),
    unsigned int index_l,
    unsigned int index_r
    ) {
    for (unsigned int i = index_l; i <= index_r; i ++) {
        if (equals(vec[i], key)) return i;
    }
    return -1;
}

int str_vector_find(vector<string> vec, string key) {
    for (unsigned int i = 0; i < vec.size(); i ++) {
        if (vec[i] == key) return i;
    }
    return -1;
}
