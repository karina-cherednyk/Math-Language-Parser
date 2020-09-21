#include "Expression.h"

ExpressionArray::ExpressionArray(ExpressionArray&& moving) :_size(moving._size), _capacity(moving._capacity) {
	_arr = moving._arr;
	moving._size = 0;
	moving._arr = nullptr;
}

void ExpressionArray::add(Expression* expr)
{
	if (_size + 1 > _capacity) enlarge();
	_arr[_size++] = expr;
}

void ExpressionArray::enlarge()
{
	_capacity = 2 * _capacity;
	Expression** arr = new Expression * [_capacity];
	for (int i = 0; i < _size; i++)
		arr[i] = _arr[i];
	delete[] _arr;
	_arr = arr;
}

ExpressionArray::~ExpressionArray()
{
	for (int i = 0; i < _size; i++)
		delete _arr[i];
	delete[] _arr;
}

