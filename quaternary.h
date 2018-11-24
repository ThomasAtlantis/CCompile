#ifndef QUATERNARY_H_INCLUDED
#define QUATERNARY_H_INCLUDED

#include "public_tools.h"

typedef struct {
    char op;
    double *a;
    double *b;
    double *res;
} Quarternary;

void print_qs(vector<Quarternary> Qs);
#endif // QUATERNARY_H_INCLUDED
