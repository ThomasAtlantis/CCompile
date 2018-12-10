#ifndef PUBLIC_TOOLS_H_INCLUDED
#define PUBLIC_TOOLS_H_INCLUDED

#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <streambuf>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <set>

#define is_num(ch) (ch>='0'&&ch<='9') // �ж��ַ��Ƿ�Ϊ����
#define is_letter(ch) ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) // �ж��ַ��Ƿ�Ϊ��ĸ
#define is_blank(ch) (ch==' '||ch=='\n'||ch=='\t'||ch=='\r') // �ж��ַ��Ƿ�Ϊ�հ׷�

#define WHITE_ON_BLACK FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY
#define BLACK_ON_WHITE BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY
#define WHITE_ON_BLUE  FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE
using namespace std;

typedef struct {
    char kind; // ���ű�����
    unsigned int index; // ���ű�λ������
} Token; // �ʷ�����������Token

#endif // PUBLIC_TOOLS_H_INCLUDED
