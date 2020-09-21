#include <string>
#include <iostream>
#include "Parser.h"
#include "Visitor.h"
using namespace std;


//Developed by Cherednyk Karina

//on 30.03.20

//Version 1.0

string rules = "Calculator:\nProvided functions: min(...), max(...), log(a,b),\nln(b), lg(b), avg(....) (average), fact(n) (factorial)\n\
Arithmentic operators: +, -, *, ^(pow), /, -(unary)\n\
Comparison operators: ==, !=, >, >=, <, <=\n\
Logic operators: and, or, not/!\n\
Available constants: e, pi\n\
Nested '(' supported\n\
|value| returns absolute value\n\
To write many equations at once, separate them by ',' ';' or start with new line \n";

int main() {



		Visitor v;
		string s;
		cout << rules << endl;
		cout << "Input your equation: " << endl;
		while(getline(cin, s)){
			try {
				cout << "Result\n";
				Tokenizer t(s);
				Parser p(t);
				cout << v.interpret(p.getParsedProgram()) << endl;
				cout << "Input your equation: " << endl;
			}
			catch (exception& e) {};
		}
		

}