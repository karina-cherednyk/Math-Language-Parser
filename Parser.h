#pragma once
#include "Expression.h"
#include <cassert>


class Parser {
private:
	Tokenizer& t;
	Expression* expression();
	Expression* logical_or();
	Expression* logical_and();
	Expression* equality();
	Expression* comparison();
	Expression* addition();
	Expression* multiplication();
	Expression* unary();
	Expression* call();
	Expression* func();
	Expression* primary();

	bool match(const TokenType* tt, int n);
	bool match(const TokenType& tt);
	void consume(const TokenType& tt, const string& em);
	void consume(const TokenType* tt, int n, const string& em);

	ExpressionArray _array;
public:
	Parser(Tokenizer& tokenizer);
	inline const ExpressionArray& getParsedProgram() {
		return _array;
	}
	inline ~Parser() {}
	
};
