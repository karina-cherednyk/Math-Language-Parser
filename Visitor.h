#pragma once
#include "Expression.h"
class Visitor {
public:
	double visitUnary(const UnaryExpression* expr);
	double visitBinary(const BinaryExpression* expr);
	double visitGrouping(const GroupingExpression* expr);
	double visitLiteral(const LiteralExpression* expr);
	double visitFunction(const FunctionExpression* expr);

	double toDouble(const Token& t);

	inline double evaluate(const Expression* expr) {
		return expr->accept(*this);
	}
	DoubleArray interpret(const ExpressionArray& arr);
};
