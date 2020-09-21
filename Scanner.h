#pragma once
#include <string>
#include <vector> 
#include <map>
#include "TokenType.h"
using namespace std;

class Scanner {
private:
	string _text;
	int _current;
public:
	Scanner(const string& text) : _text(text), _current(-1) {}
public:
	const void advance() {
		if(!end())
		 ++_current;
	}
	const char& peek() {
		return _text[_current];
	}
	const char& peekNext() {
		if(!end())
			return _text[_current + 1];
		return '\0';
	}
	bool end() {
		return  _text[_current] == '\0';
	}
	string get(int start) {
		return _text.substr(start, _current);
	}
	const int& current() {
		return _current;
	}
};


class Token {
private:
	const string* _token;
	const int _line;
	const TokenType _type;
public:
	inline Token(const string& token, int line, TokenType type) :_token(new string(token)), _line(line), _type(type) {}
	inline Token(int line, TokenType type) :_token(nullptr), _line(line), _type(type) {}
	inline ~Token(){
		delete _token;
	}
	inline const TokenType& type() const{
		return _type;
	}
	inline string value() const {
		return string(*_token);
	}
};


class Tokenizer {
private:
	
	int _line;
	vector<Token> _tokens;
	Scanner s;
	int _startOfToken;
	int _current;
	static map<string, TokenType> reserved;
	class Scanner;

public:
	class TokenizingException {
	private:
		int _line;
	public:
		TokenizingException(int l) :_line(l) {};
	};
	Tokenizer(const string& text) :s(text), _line(0), _startOfToken(0),_current(-1) { 
		try{
		scanTokens(); 
		}
		catch (const TokenizingException& e) {
			throw e;
		}
	}

	bool hasNext() {
		return _current+1 < _tokens.size();
	}
	const Token& next() {
		return _tokens[++_current];
	}
	const Token& peekNext() {
		return _tokens[_current + 1];
	}
	void scanTokens() {
		while (!s.end()) {
			scanToken();
		}
	}

	void scanToken() {
		s.advance(); char fst = s.peek();
		_startOfToken = s.current();

		switch(fst) {
		case '-':
			addToken(MIN); break;
		case '+':
			addToken(PLUS); break;
		case '*':
			addToken(MULT); break;
		case '/':
			addToken(DIV); break;
		case '^':
			addToken(POW); break;
		case '(':
			addToken(OPEN); break;
		case ')':
			addToken(CLOSE); break;
		case '!':
			match('=') ? addToken(NEQ) : addToken(FACTOR);
			break;
		case '<':
			match('=') ? addToken(LESSEQ) : addToken(LESS);
			break;
		case '>':
			match('=') ? addToken(MOREEQ) : addToken(MORE);
			break;
		case '\n':
			++_line;
			break;
		case ' ':
		case '\r':
		case '\t':
			break;
		default:
			if (isDigit(fst)) addNum();
			else if (isAlpha(fst)) addIdentifier();
			else throw TokenizingException(_line);
		}


	}
	bool match(const char c) {
		if (s.peekNext() == c)
		{
			s.advance(); return true;
		}
		return false;
	}
	bool isDigit(char c) {
		return c >= '0' && c <= '9';
	}
	bool isAlpha(char c) {
		return c >= 'a' && c <= 'z' || c>= 'A' && c<= 'Z';
	}
	bool isAN(char c) {
		return isDigit(c) || isAlpha(c);
	}

	void addNum() {
		while (isDigit(s.peek())) s.advance();
		if(match('.'))
			while (isDigit(s.peek())) s.advance();

		addToken(NUM, s.get(_startOfToken));

	}

	void addIdentifier() {
		while (isAN(s.peek())) s.advance();
		string word = s.get(_startOfToken);
		if (reserved.count(word)) addToken(reserved[word]);
		else addToken(ID, word);

	}

	void addToken(TokenType t, string token) {
		_tokens.push_back(Token(token, _line,t));
	}
	void addToken(TokenType t) {
		_tokens.push_back(Token(_line, t));
	}
};

map<string, TokenType> Tokenizer::reserved = {
	{"or",OR}, {"and",AND}, {"if",IF}, {"else",ELSE}, {"while", WHILE}, {"true",TRUE},{"false",FALSE}
};