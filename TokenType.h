#pragma once
#include <string>
#include "Function.h"
using namespace std;
enum TokenType { NUM, OR, AND,NOT, EQ, NEQ, LESS, MORE, LESSEQ, MOREEQ, MIN, PLUS,  MULT, DIV, POW,  TRUE, FALSE, ID, OPEN, CLOSE, COMMA, FUNC_ID, EOE, EOT, MOD_OPEN, MOD};
const Function* const findFunc(const string& id);

bool isReservedWord(const string& str);
bool isFunction(const string& str);
bool isIdentifier(const string& str);
TokenType getReservedType(const string& str);
double getIdValue(const string& str);
