#pragma once

#include "Common.h"
#include "Lex.h"
#include "Instr.h"

typedef map<string, int> VarMap;

// function definition
class Func
{
	int type;
	string name;
	VarMap args;
	InstrList body;
	bool _isExtern; // defined in other file
	
public:
	Func() {} // need for map
	Func(int type, const string& name, const VarMap& args, const InstrList& body)
		: type(type), name(name), args(args), body(body), _isExtern(false) {}
	Func(int type, const string& name, const VarMap& args)
		: type(type), name(name), args(args), body(body), _isExtern(true) {}
	
	int getType() const					{ return type; }
	const string& getName() const		{ return name; }
	const VarMap& getArgs() const		{ return args; }
	const InstrList& getBody() const	{ return body; }
	
	bool isExtern() const				{ return _isExtern; }
};

typedef map<string, Func> FuncMap;

class Program
{
	FuncMap funcs;
	
public:
	Program(const FuncMap& funcs) : funcs(funcs) {}
	
	const FuncMap& getFuncs() const { return funcs; }
};

class Parser
{
	Lex *lex;
	Token token;
	Program *program;
	
public:
	Parser();
	Parser(Lex *);
	void setLex(Lex *);
	const Program parse();
	
private:
	void _pass(int type, const char *tokenName);
	
	void parseProgram(FuncMap& funcs);
	void parseFuncs(FuncMap& funcs);
	bool isType();
	int parseType();
	void parseFunc(FuncMap& funcs);
	void parseArgs(VarMap& args);
	void parseArgs_(VarMap& args);
	void parseArg(VarMap& args);
	void parseBody(InstrList& instrs);
	void parseBlock(InstrList& instrs);
	void parseList(InstrList& instrs);
	void parseInstr(InstrList& instrs);
	void parseDefinitions(InstrList& instrs);
	void parseDefinitions_(InstrList& instrs, int type);
	void parseDefinition(InstrList& instrs, int type);
	void parseAssignOrFuncCall(InstrList& instrs);
	void parseExprs(ExprList& exprs);
	void parseExprs_(ExprList& exprs);
	void parseReturn(InstrList& instrs);
	void parseIf(InstrList& instrs);
	void parseWhile(InstrList& instrs);
	void parseFor(InstrList& instrs);
	const Expr *parseForCond();
	void parsePrint(InstrList& instrs);
	
	const Expr *parseExpr();
	const Expr *parseDisjunction();
	const Expr *parseConjunction();
	const Expr *parseEquality();
	const Expr *parseRelation();
	const Expr *parseAddition();
	const Expr *parseMultiplication();
	const Expr *parseExponentiation();
	const Expr *parseUnary();
	const Expr *parseAtom();
};