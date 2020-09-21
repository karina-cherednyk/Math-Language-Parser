#include "Visitor.h"
#include <cmath>
double UnaryExpression::accept(Visitor& v) const {
	return v.visitUnary(this);
}
double BinaryExpression::accept(Visitor& v) const {
	return v.visitBinary(this);
}
double GroupingExpression::accept(Visitor& v) const {
	return v.visitGrouping(this);
}
double ModExpression::accept(Visitor& v) const {
	return abs(v.visitGrouping(this));
}
double LiteralExpression::accept(Visitor& v) const {
	return v.visitLiteral(this);
}
double FunctionExpression::accept(Visitor& v) const {
	return v.visitFunction(this);
}

double Visitor::visitFunction(const FunctionExpression* expr) {
	const Function* const f = expr->_func;
	DoubleArray args;
	const ExpressionArray& eargs = expr->_args;
	for (int i = 0; i < eargs.size(); i++) 
		args.add(evaluate(eargs[i]));
	string* error = nullptr;
	double res = f->evaluate(args,error); 
	if (error != nullptr){
		string message = *error;
		delete error;
		throw CalculatingException(message,-1);
	}
	return res;
}


double Visitor::visitUnary(const UnaryExpression* expr) {
	double right = evaluate(expr->_rightOp);
	if (expr->_operator == MIN)
		return -right;
	if (expr->_operator == NOT)
		return !right;
	return -1; //unreachable
}

double Visitor::visitBinary(const BinaryExpression* expr) {
	double left = evaluate(expr->_leftOp);
	double right = evaluate(expr->_rightOp);
	switch (expr->_operator) {
	case MIN:
		return left - right;
	case PLUS:
		return left + right;
	case MULT:
		return left * right;
	case POW:
		return pow(left,right);
	case DIV:
		return left / right;
	case AND:
		return left && right;
	case OR:
		return left || right;
	case MORE:
		return left > right;
	case MOREEQ:
		return left >= right;
	case LESS:
		return left < right;
	case LESSEQ:
		return left <= right;

	case EQ:
		return left == right;
	case NEQ:
		return left != right;
	}

	return -1; //unreachable

}

double Visitor::visitGrouping(const GroupingExpression* expr) {
	return evaluate(expr->_expr);
}

double Visitor::visitLiteral(const LiteralExpression* expr) {
	return toDouble(expr->_val);
}

double Visitor::toDouble(const Token& t) {
	if (t.type() == TRUE) return 1;
	if (t.type() == FALSE) return 0;
	if (t.type() == ID) return getIdValue(t.value());
	return stod(t.value());
}
#include <iostream>
DoubleArray Visitor::interpret(const ExpressionArray& arr)
{
	DoubleArray da;
	for (int i = 0; i < arr.size(); i++)
	{ 
		try {
			da.add(evaluate(arr[i]));
		}
		catch (const CalculatingException& e) { cout << e << endl; }
	}
	return da;
}




