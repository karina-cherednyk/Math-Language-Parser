#pragma once
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
class DoubleArray {
private:
	double* _arr;
	size_t _size;
	size_t _capacity;
public:
	inline DoubleArray() : _size(0), _capacity(2), _arr(new double[2]) {};
	DoubleArray(const DoubleArray& arr);
	DoubleArray(DoubleArray&& arr);
	inline ~DoubleArray() { 
		delete[] _arr; 
	}
	void add(double d);
	void enlarge();
	inline size_t size() const { return _size; }
	inline double operator[](const size_t i) const{
		return _arr[i];
	}

};
ostream& operator<< (ostream& os, const DoubleArray& da);
inline bool isInteger(double a) {
	return floor(a) == ceil(a);
}

class Function {

public:
	const size_t _arity;
	inline Function(size_t arity):_arity(arity){}
	virtual double evaluate(const DoubleArray& args, string*& error) const= 0;
	inline virtual ~Function() {}
	virtual bool wrongArity(size_t ar) const { return ar != _arity; };
};

class Ln: public Function {
public:
	inline Ln() :Function(1){}
	inline virtual double evaluate(const DoubleArray& args,  string*& error) const {
		if (args[0] <= 0) {
			error = new string("Argument for logarithmic function must be  b>0, got: " + std::to_string(args[0]));
			return -1;
		}
		return log(args[0]);
	}
	inline virtual ~Ln(){}
};
class Lg : public Function {
public:
	inline Lg() :Function(1) {}
	inline virtual double evaluate(const DoubleArray& args, string*& error) const {
		if (args[0] <= 0) {
			error = new string("Argument for logarithmic function must be  b>0, got: " +  std::to_string(args[0]));
			return -1;
		}
		return log10(args[0]);
	}
	inline virtual ~Lg(){}
};
class Sqrt : public Function {
public:
	inline Sqrt() :Function(1) {}
	inline virtual double evaluate(const DoubleArray& args, string*& error) const {
		if (args[0] < 0) {
			error = new string("Square root function argument must >= 0, got: " + std::to_string(args[0]));
			return -1;
		}
		return sqrt(args[0]);
	}
	inline virtual ~Sqrt() {}
};
class Log : public Function {
public:
	inline Log() :Function(2) {}
	inline virtual double evaluate(const DoubleArray& args, string*& error) const {
		if (args[0] <= 1 || args[1] <=0 ) {
			error = new string("Arguments for logarithmic function must be a>1, b>0, got: " + std::to_string(args[0])+", "+ std::to_string(args[0]));
			return -1;
		}
		if (args[1] == 1) return 0;
		return log(args[1]) / log(args[0]);
	}
	inline virtual ~Log() {}
};
class Factorial : public Function {
public:
	inline Factorial() :Function(1) {}
	virtual double evaluate(const DoubleArray& args, string*& error) const;
	inline virtual ~Factorial() {}
};
class Max : public Function {
public:
	inline Max() :Function(1) {}
	virtual double evaluate(const DoubleArray& args, string*& error) const;
	virtual bool wrongArity(size_t ar) const{ return ar == 0; };
	inline virtual ~Max() {}
};
class Min : public Function {
public:
	inline Min() :Function(1) {}
	virtual double evaluate(const DoubleArray& args, string*& error) const;
	virtual bool wrongArity(size_t ar) const{ return ar == 0; };
	inline virtual ~Min() {}
};
class Average : public Function {
public:
	inline Average() :Function(1) {}
	virtual double evaluate(const DoubleArray& args, string*& error) const;
	virtual bool wrongArity(size_t ar) const{ return ar == 0; };
	inline virtual ~Average() {}
};
