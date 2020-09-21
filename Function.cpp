#include "Function.h"

DoubleArray::DoubleArray(const DoubleArray& arr):_arr(new double[arr._capacity]), _size(arr._size), _capacity(arr._capacity)
{
	for (int i = 0; i < arr.size(); i++) _arr[i] = arr._arr[i];
}
DoubleArray::DoubleArray(DoubleArray&& arr) : _arr(arr._arr), _size(arr._size), _capacity(arr._capacity)
{
	arr._arr = nullptr;
}
void DoubleArray::add(double d) {
	if (_size + 1 > _capacity) enlarge();
	_arr[_size++] = d;
}

void DoubleArray::enlarge() {
	_capacity *= 2;
	double* arr = new double[_capacity];
	for (int i = 0; i < _size; i++) arr[i] = _arr[i];
	delete[] _arr;
	_arr = arr;
}

 double Average::evaluate(const DoubleArray& args, string*& error) const
{
	double sum = 0;
	for (int i = 0; i < args.size(); i++) sum += args[i];
	return sum / args.size();
}

 double Min::evaluate(const DoubleArray& args, string*& error) const {
	 double max = args[0];
	 for (int i = 1; i < args.size(); i++) if (args[i] < max) max = args[i];
	 return max;
 }

 double Max::evaluate(const DoubleArray& args, string*& error) const {
	 double max = args[0];
	 for (int i = 1; i < args.size(); i++) if (args[i] > max) max = args[i];
	 return max;
 }

 double Factorial::evaluate(const DoubleArray& args, string*& error) const {
	 if (!isInteger(args[0]))
	 {
		 error = new string("Factorial function argument must be integer, got: " + std::to_string(args[0]));
		 return -1;
	 }
	 if (args[0] < 0) {
		 error = new string("Factorial function argument must be integer, got: " + std::to_string(args[0]));
		 return -1;
	 }
	 int x = args[0];
	 int res = 1;
	 for (int i = 1; i <= x; i++) res *= i;
	 return res;
 }

 ostream& operator<<(ostream& os, const DoubleArray& da)
 {
	 for (int i = 0; i < da.size(); i++) os << da[i] << endl;
	 return os;
 }
