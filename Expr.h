#pragma once

#include "Common.h"

// expression types
enum
{
	E_VAR,		// variable
	
	E_BOOL,		// boolean constant
	E_INT,		// integer constant
	E_FLOAT,	// float constant
	E_STRING,	// string constant
	
	// numeric operators
	E_SUM,		// expr + expr
	E_SUB,		// expr - expr
	E_MUL,		// expr * expr
	E_POW,		// expr ** expr
	E_DIV,		// expr / expr
	E_MOD,		// expr % expr
	E_NEG,		// -expr
	
	// logic operators
	E_AND,		// expr and expr
	E_OR,		// expr or expr
	E_NOT,		// !expr
	
	// relation operators
	E_EQ,		// expr = expr
	E_NE,		// expr != expr
	E_LT,		// expr < expr
	E_LE,		// expr <= expr
	E_GT,		// expr > expr
	E_GE,		// expr >= expr
	
	E_FUNC_CALL	// func(exprs)
};

// general expression interface
class Expr
{
public:
	virtual int getID() const = 0;
};
// general unary expression interface
class ExprUnary : public Expr
{
public:
	virtual int getID() const = 0;
	
	virtual const Expr *getValue() const = 0;
};
// general binary expression interface
class ExprBinary : public Expr
{
public:
	virtual int getID() const = 0;
	
	virtual const Expr *getFirst()  const = 0;
	virtual const Expr *getSecond() const = 0;
};

typedef list<const Expr *> ExprList;

// constant
template<int id, typename T>
class _ExprConst : public Expr
{
	T value;
	
public:
	virtual int getID() const { return id; }
	
	_ExprConst(const T& value) : value(value) {}
	
	const T& getValue() const { return value; }
};

// unary operator
template<int id>
class _ExprUnary : public ExprUnary
{
	const Expr *value;
	
public:
	virtual int getID() const { return id; }
	
	_ExprUnary(const Expr *value) : value(value) {}
	
	const Expr *getValue() const { return value; }
};

// binary operator
template<int id>
class _ExprBinary : public ExprBinary
{
	const Expr *first;
	const Expr *second;

public:
	virtual int getID() const { return id; }
	
	_ExprBinary(const Expr *first, const Expr *second)
		: first(first), second(second) {}
	
	const Expr *getFirst()  const { return first;  }
	const Expr *getSecond() const { return second; }
};

// function call
template<int id>
class _ExprFuncCall : public Expr
{
	string name;
	ExprList args;
	
public:
	virtual int getID() const { return id; }

	_ExprFuncCall(const string& name, const ExprList& args)
		: name(name), args(args) {}
	
	const string&   getName() const { return name; }
	const ExprList& getArgs() const { return args; }
};

typedef _ExprConst <E_VAR, string>		Var;		// variable

typedef _ExprConst <E_BOOL,   bool>		Bool;		// boolean constant
typedef _ExprConst <E_INT,    int>		Int;		// integer constant
typedef _ExprConst <E_FLOAT,  float>	Float;		// float constant
typedef _ExprConst <E_STRING, string>	String;		// string constant

// numeric operators
typedef _ExprBinary<E_SUM>				Sum;		// expr + expr
typedef _ExprBinary<E_SUB>				Sub;		// expr - expr
typedef _ExprBinary<E_MUL>				Mul;		// expr * expr
typedef _ExprBinary<E_POW>				Pow;		// expr ** expr
typedef _ExprBinary<E_DIV>				Div;		// expr / expr
typedef _ExprBinary<E_MOD>				Mod;		// expr % expr
typedef _ExprUnary <E_NEG>				Neg;		// -expr

// logic operators
typedef _ExprBinary<E_AND>				And;		// expr and expr
typedef _ExprBinary<E_OR>				Or;			// expr or expr
typedef _ExprUnary <E_NOT>				Not;		// !expr

// relation operators
typedef _ExprBinary<E_EQ>				Eq;			// expr = expr
typedef _ExprBinary<E_NE>				Ne;			// expr != expr
typedef _ExprBinary<E_LT>				Lt;			// expr < expr
typedef _ExprBinary<E_LE>				Le;			// expr <= expr
typedef _ExprBinary<E_GT>				Gt;			// expr > expr
typedef _ExprBinary<E_GE>				Ge;			// expr >= expr

typedef _ExprFuncCall<E_FUNC_CALL>		EFuncCall;	// func(exprs)