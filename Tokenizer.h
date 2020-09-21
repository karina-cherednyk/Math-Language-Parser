#pragma once
#include <string>
#include <vector> 
#include <map>
#include <set>
#include "TokenType.h"
#include "CalculatingException.h"
using namespace std;

class Scanner {
private:
	string _text;
	int _current;
public:
	Scanner(const string& text);
public:
	void advance();
	void back();
	const char& peek();
	const char& peekNext();
	bool end();
	string get(int start);
	const int& current();
};


class Token {
private:
	const string* _value;
	const int _line;
	const TokenType _type;
public:
	Token(const Token&) = delete;
	inline Token(Token&& moving):_value(moving._value),_line(moving._line),_type(moving._type) {
	
	}
	inline Token(const string& token, int line, TokenType type) :_value(new string(token)), _line(line), _type(type) {}
	inline Token(int line, TokenType type) :_value(nullptr), _line(line), _type(type) {}
	~Token(){}
	const TokenType& type() const{
		return _type;
	}
	inline const string& value() const {
		return *_value;
	}
	inline const int& line() const {
		return _line;
	}
	operator string() const;
};


class Tokenizer {
private:
	
	int _line;
	vector<Token> _tokens;
	Scanner s;
	int _startOfToken;
	int _current;
	static set<string> funcMap;

public:
	Tokenizer(const string& text);

	bool hasNext();
	const Token& next();
	const Token& peekNext();
	void scanTokens();

	void scanToken();
	bool match(const char c);
	bool isDigit(char c);
	bool isAlpha(char c);
	bool isAN(char c);

	void addNum();

	void addIdentifier();

	void addToken(TokenType t, const string& token);
	void addToken(TokenType t);
};

