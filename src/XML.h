#pragma once

#include "Parser.h"

using std::cout;
using std::endl;

class TagsInstrs : public map<int, string>
{
public:
	TagsInstrs()
	{
		add(I_VAR_ASSIGN,		"assignment");
		add(I_VAR_SUM_ASSIGN,	"summation-assignment");
		add(I_VAR_SUB_ASSIGN,	"subtraction-assignment");
		add(I_VAR_MUL_ASSIGN,	"multiplication-assignment");
		add(I_VAR_POW_ASSIGN,	"exponentiation-assignment");
		add(I_VAR_DIV_ASSIGN,	"division-assignment");
		add(I_VAR_MOD_ASSIGN,	"modulo-assignment");
		
		add(I_VAR_INC,			"increment");
		add(I_VAR_DEC,			"decrement");
	}
	
private:
	void add(int id, const string& name)
	{
		insert(std::pair<int, string>(id, name));
	}
};

class TagsExprs : public map<int, string>
{
public:
	TagsExprs()
	{
		add(E_SUM,	"summation");
		add(E_SUB,	"subtraction");
		add(E_MUL,	"multiplication");
		add(E_POW,	"exponentiation");
		add(E_DIV,	"division");
		add(E_MOD,	"modulo");
		add(E_NEG,	"negation");

		add(E_AND,	"and");
		add(E_OR,	"or");
		add(E_NOT,	"not");

		add(E_EQ,	"equal");
		add(E_NE,	"not-equal");
		add(E_LT,	"less");
		add(E_LE,	"less-or-equal");
		add(E_GT,	"greater");
		add(E_GE,	"greater-or-equal");
	}
	
private:
	void add(int id, const string& name)
	{
		insert(std::pair<int, string>(id, name));
	}
};

class XML
{
	TagsInstrs tagsInstrs;
	TagsExprs  tagsExprs;
	
public:
	void printProgram(const Program& program);
	void printFunc(const Func& func);
	void printInstrs(const InstrList& instrs, int t);
	void printInstr(const Instr *i, int t);
	void printVarDef(const VarDef *i, int t);
	void printVarAssign(const InstrVarAssign *i, int t);
	void printVarIncDec(const InstrVarIncDec *i, int t);
	void printReturn(const Return *i, int t);
	void printIFuncCall(const IFuncCall *i, int t);
	void printIf(const If *i, int t);
	void printWhile(const While *i, int t);
	void printFor(const For *i, int t);
	void printPrint(const Print *i, int t);
	void printExpr(const Expr *e, int t);
	void printVar(const Var *e, int t);
	void printBool(const Bool *e, int t);
	void printInt(const Int *e, int t);
	void printFloat(const Float *e, int t);
	void printString(const String *e, int t);
	void printBinary(const ExprBinary *e, int t);
	void printUnary(const ExprUnary *e, int t);
	void printEFuncCall(const EFuncCall *e, int t);
	const string typeName(int type);
	void tabs(int num);
};