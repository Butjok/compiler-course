#pragma once

#include "Common.h"
#include "Expr.h"

// instruction types
enum
{
	I_VAR_DEF,			// type var [= expr];
	
	I_VAR_ASSIGN,		// var = expr;
	I_VAR_SUM_ASSIGN,	// var += expr;
	I_VAR_SUB_ASSIGN,	// var -= expr;
	I_VAR_MUL_ASSIGN,	// var *= expr;
	I_VAR_POW_ASSIGN,	// var **= expr;
	I_VAR_DIV_ASSIGN,	// var /= expr;
	I_VAR_MOD_ASSIGN,	// var %= expr;
	
	I_VAR_INC,			// var++;
	I_VAR_DEC,			// var--;
	
	I_RETURN,			// return [expr];
	
	I_FUNC_CALL,		// func(exprs);
	
	I_IF,				// if (cond) body [ else body ]
	
	I_WHILE,			// while (cond) body
	I_FOR,				// for (init; cond; step;) body
	
	I_PRINT				// print(exprs);
};

// general instruction interface
class Instr
{
public:
	virtual int getID() const = 0;
};
// general variable assignment instruction interface
class InstrVarAssign : public Instr
{
public:
	virtual int getID() const = 0;
	
	virtual const string& getName()  const = 0;
	virtual const Expr *  getValue() const = 0;
};
// general variable increment/decrement instruction interface
class InstrVarIncDec : public Instr
{
public:
	virtual int getID() const = 0;
	
	virtual const string& getName() const = 0;
};

typedef list<const Instr *> InstrList;

// type var [= expr];
template<int id>
class _InstrVarDef : public Instr
{
	int type;
	string name;
	const Expr *value;
	
public:
	virtual int getID() const { return id; }
	
	_InstrVarDef() {} // need for map
	_InstrVarDef(int type, const string& name)
		: type(type), name(name), value(0) {}
	_InstrVarDef(int type, const string& name, const Expr *value)
		: type(type), name(name), value(value) {}

	int           getType()  const { return type;  }
	const string& getName()  const { return name;  }
	const Expr *  getValue() const { return value; }
};

// var = expr;
template<int id>
class _InstrVarAssign : public InstrVarAssign
{
	string name;
	const Expr *value;
	
public:
	virtual int getID() const { return id; }
	
	_InstrVarAssign(const string& name, const Expr *value)
		: name(name), value(value) {}
		
	virtual const string& getName()  const { return name;  }
	virtual const Expr *  getValue() const { return value; }
};

// var++; or var--;
template<int id>
class _InstrVarIncDec : public InstrVarIncDec
{
	string name;
	
public:
	virtual int getID() const { return id; }
	
	_InstrVarIncDec(const string& name) : name(name) {}
		
	virtual const string& getName() const { return name; }
};

// return [expr];
template<int id>
class _InstrReturn : public Instr
{
	const Expr *value;
	
public:
	virtual int getID() const { return id; }
	
	_InstrReturn() : value(0) {}
	_InstrReturn(const Expr *value) : value(value) {}
	
	const Expr *getValue() const { return value; }
};

// func(exprs)
template<int id>
class _InstrFuncCall : public Instr
{
	string name;
	ExprList args;
	
public:
	virtual int getID() const { return id; }
	
	_InstrFuncCall(const string& name, const ExprList& args)
		: name(name), args(args) {}
		
	const string& getName() const	{ return name; }
	const ExprList& getArgs() const	{ return args; }
};

// if (cond) body [ else body ]
template<int id>
class _InstrIf : public Instr
{
	const Expr *cond;
	InstrList trueBody;
	InstrList falseBody;
	
public:
	virtual int getID() const { return id; }
	
	_InstrIf(const Expr *cond, const InstrList& trueBody, const InstrList& falseBody)
		: cond(cond), trueBody(trueBody), falseBody(falseBody) {}
	
	const Expr *     getCond()      const { return cond;      }
	const InstrList& getTrueBody()  const { return trueBody;  }
	const InstrList& getFalseBody() const { return falseBody; }
};

// while (cond) body
template<int id>
class _InstrWhile : public Instr
{
	const Expr *cond;
	InstrList body;
	
public:
	virtual int getID() const { return id; }

	_InstrWhile(const Expr *cond, const InstrList& body)
		: cond(cond), body(body) {}
	
	const Expr *     getCond() const { return cond; }
	const InstrList& getBody() const { return body; }
};

// for (init; cond; step;) body
template<int id>
class _InstrFor : public Instr
{
	InstrList init;
	const Expr *cond;
	InstrList step;
	InstrList body;
	
public:
	virtual int getID() const { return id; }

	_InstrFor(const InstrList& init, const Expr *cond, const InstrList& step, const InstrList& body)
		: init(init), cond(cond), step(step), body(body) {}
	
	const InstrList& getInit() const { return init; }
	const Expr *     getCond() const { return cond; }
	const InstrList& getStep() const { return step; }
	const InstrList& getBody() const { return body; }
};

// print(exprs);
template<int id>
class _InstrPrint : public Instr
{
	ExprList args;
	
public:
	virtual int getID() const { return id; }
	
	_InstrPrint(const ExprList& args) : args(args) {}
	
	const ExprList& getArgs() const { return args; }
};

typedef _InstrVarDef   <I_VAR_DEF>			VarDef;			// type var [= expr];

typedef _InstrVarAssign<I_VAR_ASSIGN>		VarAssign;		// var = expr;
typedef _InstrVarAssign<I_VAR_SUM_ASSIGN>	VarSumAssign;	// var += expr;
typedef _InstrVarAssign<I_VAR_SUB_ASSIGN>	VarSubAssign;	// var -= expr;
typedef _InstrVarAssign<I_VAR_MUL_ASSIGN>	VarMulAssign;	// var *= expr;
typedef _InstrVarAssign<I_VAR_POW_ASSIGN>	VarPowAssign;	// var **= expr;
typedef _InstrVarAssign<I_VAR_DIV_ASSIGN>	VarDivAssign;	// var /= expr;
typedef _InstrVarAssign<I_VAR_MOD_ASSIGN>	VarModAssign;	// var %= expr;

typedef _InstrVarIncDec<I_VAR_INC>			VarInc;			// var++;
typedef _InstrVarIncDec<I_VAR_DEC>			VarDec;			// var--;

typedef _InstrReturn   <I_RETURN>			Return;			// return [expr];

typedef _InstrFuncCall <I_FUNC_CALL>		IFuncCall;		// func(exprs);

typedef _InstrIf       <I_IF>				If;				// if (cond) body [ else body ]

typedef _InstrWhile    <I_WHILE>			While;			// while (cond) body
typedef _InstrFor      <I_FOR>				For;			// for (init; cond; step;) body

typedef _InstrPrint    <I_PRINT>			Print;			// print(exprs);