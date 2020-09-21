#define _USE_MATH_DEFINES
#include "TokenType.h"
#include <map>
#include <cmath>
using namespace std;
Factorial m_fact;
Log m_log;
Ln m_ln;
Lg m_lg;
Sqrt m_sqrt;
Max m_max;
Min m_min;
Average m_average;
map<string, Function*> funcs = {
	{"fact",&m_fact}, {"log",&m_log},{"sqrt",&m_sqrt},{"lg",&m_lg},{"ln",&m_ln}, {"min", &m_min}, {"max",&m_max},{"avg",&m_average}
};
map<string, TokenType> reserved = {
	{"or",OR}, {"and",AND},/* {"if",IF}, {"else",ELSE}, {"while", WHILE}, */{"true",TRUE},{"false",FALSE},{"not",NOT}
};
map<string, double> ids = { { "e", 	M_E },{ "pi", M_PI} };

const Function* const findFunc(const string& id) {
	if (funcs.find(id) != funcs.end())return funcs[id];
	return nullptr;
}
bool isReservedWord(const string& str) {
	return reserved.find(str) != reserved.end();
}
bool isFunction(const string& str) {
	return findFunc(str) != nullptr;
}
bool isIdentifier(const string& str) {
	return ids.find(str) != ids.end();
}
TokenType getReservedType(const string& str) {
	return reserved[str];
}
double getIdValue(const string& str) {
	return ids[str];
}

