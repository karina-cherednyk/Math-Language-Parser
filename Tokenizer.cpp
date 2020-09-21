#include "Tokenizer.h"
#include <iostream>


Scanner::Scanner(const string& text) : _text(text), _current(0) {}

void Scanner::advance() {
	//if (!end())
		++_current;
}
void Scanner::back() {
	--_current;
}
const char& Scanner::peek() {
	return _text[_current];
}
const char& Scanner::peekNext() {
	if (!end())
		return _text[_current + 1];
	return '\0';
}
bool Scanner::end() {
	return  _text[_current] == '\0';
}
string Scanner::get(int start) {
	return _text.substr(start, (_current - start));
}
const int& Scanner::current() {
	return _current;
}



Tokenizer::Tokenizer(const string& text) :s(text), _line(1), _startOfToken(0), _current(-1) {
	try {
		scanTokens();
	}
	catch (const CalculatingException& e) {
		cout << e << endl;
		throw e;
	}
}


bool Tokenizer::hasNext() {
	return _current + 1 < _tokens.size() && peekNext().type() != EOT;
}
const Token& Tokenizer::next() {
	return _tokens[++_current];
}
const Token& Tokenizer::peekNext() {
	return _tokens[_current + 1];
}
void Tokenizer::scanTokens() {
	while (!s.end()) {
		scanToken();
		s.advance();
	}
	addToken(EOT);
}

void Tokenizer::scanToken() {
	char fst = s.peek();
	_startOfToken = s.current();

	switch (fst) {
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
	case '|':
		addToken(MOD); break;
	case ',':
		addToken(COMMA); break;
	case '!':
		match('=') ? addToken(NEQ) : addToken(NOT);
		break;
	case '<':
		match('=') ? addToken(LESSEQ) : addToken(LESS);
		break;
	case '>':
		match('=') ? addToken(MOREEQ) : addToken(MORE);
		break;
	case '=':
		match('=') ? addToken(EQ) : throw CalculatingException("Expected '==' sign",_line);
		break;
	case ';':
	case '\n':
		++_line;
		addToken(EOE);
		break;
	case ' ':
	case '\r':
	case '\t':
		break;
	default:
		if (isDigit(fst)) addNum();
		else if (isAlpha(fst)) addIdentifier();
		else throw CalculatingException(" Undefined char at position "+std::to_string(_startOfToken)+": "+fst,_line );
	}


}
bool Tokenizer::match(const char c) {
	if (s.peekNext() == c)
	{
		s.advance(); return true;
	}
	return false;
}
bool Tokenizer::isDigit(char c) {
	return c >= '0' && c <= '9';
}
bool Tokenizer::isAlpha(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}
bool Tokenizer::isAN(char c) {
	return isDigit(c) || isAlpha(c);
}

void Tokenizer::addNum() {
	while (isDigit(s.peek())) s.advance();
	if (match('.'))
		while (isDigit(s.peek())) s.advance();

	addToken(NUM, s.get(_startOfToken));

	s.back();
}

void Tokenizer::addIdentifier() {
	while (isAN(s.peek())) s.advance();
	string word = s.get(_startOfToken);
	if (isReservedWord(word)) addToken(getReservedType(word));
	else { 
		if (isFunction(word)) addToken(FUNC_ID, word);
		else if (isIdentifier(word)) addToken(ID, word);
		else 	throw CalculatingException( "Expected identifier, got " + word,_line);
		
	}
	
	s.back();
}

void Tokenizer::addToken(TokenType t, const string& token) {
	_tokens.push_back(std::move(Token(token, _line, t)));
}
void Tokenizer::addToken(TokenType t) {
	_tokens.push_back(std::move(Token(_line, t)));
}


Token::operator string() const
{
	const TokenType& t = type();
	switch (t) {
	case NUM: return "number: "+value();
	case OR: return "'or' operator";
	case AND: return "'and' operator";
	case NOT: return "'not' operator";
	case EQ: return "'==' sign";
	case NEQ: return "'!=' sign";
	case LESS: return "'<' sign";
	case MORE: return "'>' sign";
	case LESSEQ: return "'<=' sign";
	case MOREEQ: return "'>=' sign";
	case MIN: return "'-' sign";
	case PLUS: return "'+' sign";
	case MULT: return "'*' sign";
	case DIV: return "'\' sign";
	case POW: return "'^' sign";
	case TRUE: return "true boolean";
	case FALSE: return "false boolean";
	case ID: return "constant value: "+value();
	case OPEN: return "'('";
	case CLOSE: return "')'";
	case COMMA: return "','";
	case FUNC_ID: return "function identifier: "+value();
	case EOE: return "end of expression";
	case EOT: return "end of commands";
	}
}
