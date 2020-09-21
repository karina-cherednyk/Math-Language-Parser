#include "Parser.h"
#include <iostream>
//literal    → NUM | "true" | "false" ;
//grouping   → "(" expression ")";
//unary      →("-" | "!") expression;
//binary     → expression operator expression;
//operator → "==" | "!=" | "<" | "<=" | ">" | ">="
//| "+" | "-" | "*" | "/";


//UPDATE:
//expression -> logic_or;
//logic_or -> logic_and (OR logic_and)*;
//logic_and -> equality (AND equality)*;
//equality       -> comparison(("!=" | "==") comparison)*;
//comparison     -> addition((">" | ">=" | "<" | "<=") addition)*;
//addition       -> multiplication("-" | "+") multiplication)*;
//multiplication -> unary("/" | "*" | "^" ) unary)*;
//unary          -> ("!" | "-") unary | call;
//call -> func_ID OPEN arguments CLOSE | primary;
//arguments -> ( expression ( COMMA expression )* )?;
//primary        -> NUMBER | ID | "false" | "true" | "(" expression ")";




Expression* Parser::expression() {
	return logical_or();
}

Expression* Parser::logical_or() {
	Expression* expr = logical_and();
	TokenType awaitTypes[] = { OR};
	while (match(awaitTypes, 1)) {
		const TokenType& oper = t.next().type();
		Expression* right = logical_and();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;
}
Expression* Parser:: logical_and() {
	Expression* expr = equality();

	TokenType awaitTypes[] = { AND };
	while (match(awaitTypes, 1)) {
		const TokenType& oper = t.next().type();
		Expression* right = equality();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;
}

Expression* Parser::equality() {
	Expression* expr = comparison();

	TokenType awaitTypes[] = { EQ,NEQ };
	while (match(awaitTypes, 2)) {
		const TokenType& oper = t.next().type();
		Expression* right = comparison();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;
}

Expression* Parser::comparison() {
	Expression* expr = addition();
	TokenType awaitTypes[] = { MORE, MOREEQ, LESS, LESSEQ };
	while (match(awaitTypes, 4)) {
		const TokenType& oper = t.next().type();
		Expression* right = addition();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;

}
Expression* Parser::addition() {
	Expression* expr = multiplication();
	TokenType awaitTypes[] = { MIN,PLUS };
	while (match(awaitTypes, 2)) {
		const TokenType& oper = t.next().type();
		Expression* right = multiplication();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;
}
Expression* Parser::multiplication() {
	Expression* expr = unary();
	TokenType awaitTypes[] = { MULT,DIV,POW };
	while (match(awaitTypes, 3)) {
		const TokenType& oper = t.next().type();
		Expression* right = unary();
		expr = new BinaryExpression(expr, right, oper);
	}
	return expr;
}
Expression* Parser::unary() {
	TokenType awaitTypes[] = { MIN,NOT };
	if (match(awaitTypes, 2)) {
		const TokenType& oper = t.next().type();
		Expression* right = unary();
		return new UnaryExpression(right, oper);
	}
	else return call();
}


Expression* Parser::call() {
	if (match(FUNC_ID)) 
		return func();
	else return primary();
}
Expression* Parser::func() {
	const Token& func = t.next();
	const string& funcId = func.value();
	consume(OPEN, "Expected '(' after function id");
	ExpressionArray arguments;
	bool fst = true;
	if (!match(CLOSE)) {
		do {
			if (!fst) t.next();
			arguments.add(expression());
			fst = false;
		} while (match(COMMA));
	}
	consume(CLOSE, "Expected '(' after arguments list");
	const Function* const f = findFunc(funcId);
	size_t arity = f->_arity;
	if(f->wrongArity(arguments.size())) 
		throw CalculatingException("Wrong number of arguments for function "+funcId+"\nExprected: " + std::to_string(arity) +", got: "+ std::to_string(arguments.size()),func.line() );
	return new FunctionExpression(f,arguments);
}

Expression* Parser::primary() {
	TokenType awaitTypes[] = { FALSE, TRUE, ID, NUM };
	if (match(awaitTypes, 4)) 
		return new LiteralExpression(t.next());
	
	if (match(OPEN)) {
		t.next();
		Expression* expr = expression();
		consume(CLOSE, "Expected ')' after expression.");
		return new GroupingExpression(expr);
	}
	else if (match(MOD)) {
		t.next();
		Expression* expr = expression();
		consume(MOD, "Expected '|' after expression.");
		return new ModExpression(expr);
	}
	throw CalculatingException(string("Expected identifier or literal, got: ")+ string(t.peekNext()),t.peekNext().line() );
}
bool  Parser::match(const TokenType* tt, int n) {
	const TokenType& type = t.peekNext().type();
	for (int i = 0; i < n; i++)
		if (type == tt[i]) return true;
	return false;
}
bool Parser::match(const TokenType& tt) {
	return t.hasNext() && t.peekNext().type() == tt;

}
void Parser::consume(const TokenType* tt, int n, const string& em) {
	const TokenType& type = t.peekNext().type();
	for (int i = 0; i < n; i++)
		if (type == tt[i]) { t.next(); return; }
	throw CalculatingException(em,t.next().line());
}

void  Parser::consume(const TokenType& tt, const string& em) {
	if (t.peekNext().type() != tt) throw CalculatingException(em, t.peekNext().line());
	t.next();
}



Parser::Parser(Tokenizer& tokenizer) :t(tokenizer) {
	TokenType endExpressionTypes[] = { EOE,COMMA, EOT };
	while (t.hasNext()) {
		try {
			Expression* e = expression();
			consume(endExpressionTypes, 3, "Expected end of expression");
			_array.add(e);
		}
		catch (const CalculatingException& pe) {
			cout << pe << endl;
			while (!match(endExpressionTypes, 3)) t.next();
		}

		
		
	}
};
