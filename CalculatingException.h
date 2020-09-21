#pragma once
#include<iostream>
#include <string>
using namespace std;
class CalculatingException : public exception {
public:
	const int _line;
	const string _error;
	inline CalculatingException(const string& err, int l) :_error(err),_line(l) {}
};
inline ostream& operator<<(ostream& os, const CalculatingException& pe) {
	return os << "Text processing failed "<< ((pe._line!=-1) ? ("at expression: "+  to_string(pe._line) ):"")  << "\n Error message: " << pe._error << endl;
}
