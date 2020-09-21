#pragma once
#include "Tokenizer.h"
#include <cassert>
//expression → literal
//| unary
//| binary
//| grouping;
//
class Visitor;
class Expression {
public:
	virtual ~Expression() {}
	inline virtual double accept(Visitor&) const = 0;
};

class UnaryExpression : public Expression {
	
public:
	const Expression* _rightOp;
	const TokenType _operator;

	inline virtual ~UnaryExpression() { delete _rightOp;  }
	virtual double accept(Visitor& v) const;
	inline UnaryExpression(const Expression* expr, const TokenType& op) :_rightOp(expr), _operator(op) {}
};
class BinaryExpression :public Expression {
public:
	const Expression* _leftOp;
	const Expression* _rightOp;
	const TokenType _operator;

	inline virtual ~BinaryExpression() { delete _leftOp; delete _rightOp; }
	virtual double accept(Visitor& v) const;
	inline BinaryExpression(const Expression* left, const Expression* right, const TokenType& op) :_leftOp(left), _rightOp(right), _operator(op) {}
};
class GroupingExpression : public Expression {
public:
	const Expression* _expr;
	inline virtual ~GroupingExpression() { delete _expr; }
	virtual double accept(Visitor& v) const;
	inline GroupingExpression(const Expression* expr) :_expr(expr) {}
};
class ModExpression : public GroupingExpression {
public:

	inline virtual ~ModExpression() { }
	virtual double accept(Visitor& v) const;
	inline ModExpression(const Expression* expr) :GroupingExpression(expr) {}
};
class LiteralExpression : public Expression {
public:
	const Token& _val;

	inline virtual ~LiteralExpression() {}
	virtual double accept(Visitor& v) const;
	inline LiteralExpression(const Token& val) :_val(val){}
};


class ExpressionArray {
private:
	Expression** _arr;
	size_t _size;
	size_t _capacity;
public:
	inline ExpressionArray() :_size(0), _capacity(5), _arr(new Expression* [5]) {};
	ExpressionArray(ExpressionArray&& moving);
	ExpressionArray(const Expression&) = delete;
	void add(Expression* expr);
	void enlarge();
	~ExpressionArray();
	inline const Expression* operator[](size_t i) const{
		return _arr[i];
	}
	inline size_t size() const {
		return _size;
	}
};

#include <utility>

class FunctionExpression : public Expression {
public:
	const Function* const _func;
	const ExpressionArray _args;

	inline virtual ~FunctionExpression() {}
	virtual double accept(Visitor& v) const;
	inline FunctionExpression(const Function* const func, ExpressionArray& arr) :_func(func), _args(std::move(arr)) {}
};
