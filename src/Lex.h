#pragma once

#include "Common.h"

// token types
enum
{
	// keywords
	T_VOID,				// void
	T_BOOL,				// bool
	T_INT,				// int
	T_FLOAT,			// float
	T_TRUE,				// true
	T_FALSE,			// false
	T_AND,				// and
	T_OR,				// or
	T_IF,				// if
	T_ELSE,				// else
	T_WHILE,			// while
	T_DO,				// do
	T_FOR,				// for
	T_RETURN,			// return
	T_PRINT,			// print
	T_EXTERN,			// extern
	
	// tokens with value
	T_IDENTIFIER,		// any valid identifier
	T_INTEGER_CONSTANT,	// any valid integer value
	T_FLOAT_CONSTANT,	// any valid float value
	T_STRING,			// any enquoted string

	// equality check
	// note: "=" is assignment sign too
	T_EQ,				// =
	T_NE,				// !=
	
	T_SUM_ASSIGN,		// +=
	T_SUB_ASSIGN,		// -=
	T_MUL_ASSIGN,		// *=
	T_POW_ASSIGN,		// **=
	T_DIV_ASSIGN,		// /=
	T_MOD_ASSIGN,		// %=
	
	// relations
	T_LT,				// <
	T_LE,				// <=
	T_GT,				// >
	T_GE,				// >=
	
	// operators
	T_PLUS,				// +
	T_INC,				// ++
	T_MINUS,			// -
	T_DEC,				// --
	T_MUL,				// *
	T_POW,				// **
	T_DIV,				// /
	T_MOD,				// %
	T_NOT,				// !

	// symbols
	T_SEMICOLON,		// ;
	T_COMMA,			// ,
	T_LEFT,				// (
	T_RIGHT,			// )
	T_LEFT_CURLY,		// {
	T_RIGHT_CURLY,		// }

	// special tokens
	T_EOF				// end of file
};

class Keywords : public map<string, int>
{
public:
	Keywords()
	{
		add("void",		T_VOID);
		add("bool",		T_BOOL);
		add("int",		T_INT);
		add("float",	T_FLOAT);
		add("true",		T_TRUE);
		add("false",	T_FALSE);
		add("and",		T_AND);
		add("or",		T_OR);
		add("if",		T_IF);
		add("else",		T_ELSE);
		add("while",	T_WHILE);
		add("do", 		T_DO);
		add("for",		T_FOR);
		add("return",	T_RETURN);
		add("print",	T_PRINT);
		add("extern",	T_EXTERN);
	}
	
private:
	void add(const string& name, int type)
	{
		insert(std::pair<string, int>(name, type));
	}
};

class Token
{
	int type;
	string data;
	
	string name; // token name, for example T_EOF
	
public:
	Token() {}
	Token(int type, const string& name)
		: type(type), name(name) {}
	Token(int type, const string& data, const string& name)
		: type(type), data(data), name(name) {}
	
	int getType() const				{ return type; }
	const string& getData() const	{ return data; }
	
	bool operator==(int type) const	{ return this->type == type; }
	bool operator!=(int type) const { return !operator==(type);  }
	
	const string& getName() const	{ return name; }
};

class Lex
{
	Keywords keywords;
	
	std::istream *in;
	int c;
	
	int line, col;
	
public:
	Lex();
	Lex(std::istream *in);
	void setInput(std::istream *in);
	Token next();
	
	int getLine() const	{ return line; }
	int getCol() const	{ return col;  }
	
private:
	const string readString(char quote);
	const string readNumberOrIdentifier();
	
	bool isIntNumber(const string& data);
	bool isFloatNumber(const string& data);
	
	void skipWhitespaces();
	void skipCComment();
	void skipCppComment();
	
	void nextc();
};