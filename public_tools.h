#ifndef PUBLIC_TOOLS_H_INCLUDED
#define PUBLIC_TOOLS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <iomanip>
#include <streambuf>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#define is_num(ch) (ch>='0'&&ch<='9')
#define is_letter(ch) ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
#define is_blank(ch) (ch==' '||ch=='\n'||ch=='\t'||ch=='\r')

using namespace std;

#endif // PUBLIC_TOOLS_H_INCLUDED
