#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "public_tools.h"
#include "process_constant_num.h"
#include "process_character.h"
#include "process_character_string.h"

typedef struct {
    Token token;
    int error_type;
    string error_log;
} ScannerGet;

ScannerGet scanner(
    string& buffer,
    unsigned int& cur_index,
    unsigned int& line_label,
    vector<string>& KT,
    vector<string>& PT,
    vector<string>& IT,
    vector<char>& cT,
    vector<string>& ST,
    vector<double>& CT
);

#endif // SCANNER_H_INCLUDED
