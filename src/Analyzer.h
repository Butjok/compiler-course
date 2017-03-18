#pragma once

#include "Parser.h"

static int typesBinary[][4] =
{
	//	op		first		second		result
	
	// +
	{	E_SUM,	T_INT,		T_INT,		T_INT	},	// int   + int   = int
	{	E_SUM,	T_FLOAT,	T_FLOAT,	T_FLOAT	},	// float + float = float
	{	E_SUM,	T_INT,		T_FLOAT,	T_FLOAT	},	// int   + float = float
	{	E_SUM,	T_FLOAT,	T_INT,		T_FLOAT	},	// float + int   = float
	
	// -
	{	E_SUB,	T_INT,		T_INT,		T_INT	},	// int   - int   = int
	{	E_SUB,	T_FLOAT,	T_FLOAT,	T_FLOAT	},	// float - float = float
	{	E_SUB,	T_INT,		T_FLOAT,	T_FLOAT	},	// int   - float = float
	{	E_SUB,	T_FLOAT,	T_INT,		T_FLOAT	},	// float - int   = float
	
	// *
	{	E_MUL,	T_INT,		T_INT,		T_INT	},	// int   * int   = int
	{	E_MUL,	T_FLOAT,	T_FLOAT,	T_FLOAT	},	// float * float = float
	{	E_MUL,	T_INT,		T_FLOAT,	T_FLOAT	},	// int   * float = float
	{	E_MUL,	T_FLOAT,	T_INT,		T_FLOAT	},	// float * int   = float
	
	// **
	{	E_POW,	T_INT,		T_INT,		T_INT	},	// int   ** int   = float
	{	E_POW,	T_FLOAT,	T_FLOAT,	T_FLOAT	},	// float ** float = float
	{	E_POW,	T_INT,		T_FLOAT,	T_FLOAT	},	// int   ** float = float
	{	E_POW,	T_FLOAT,	T_INT,		T_FLOAT	},	// float ** int   = float
	
	// /
	{	E_DIV,	T_INT,		T_INT,		T_INT	},	// int   / int   = int
	{	E_DIV,	T_FLOAT,	T_FLOAT,	T_FLOAT	},	// float / float = float
	{	E_DIV,	T_INT,		T_FLOAT,	T_FLOAT	},	// int   / float = float
	{	E_DIV,	T_FLOAT,	T_INT,		T_FLOAT	},	// float / int   = float
	
	// %
	{	E_MOD,	T_INT,		T_INT,		T_INT	},	// int % int = int
	
	// and
	{	E_AND,	T_BOOL,		T_BOOL,		T_BOOL	},	// bool and bool = bool
	
	// or
	{	E_OR,	T_BOOL,		T_BOOL,		T_BOOL	},	// bool or bool = bool
	
	// =
	{	E_EQ,	T_BOOL,		T_BOOL,		T_BOOL	},	// (bool  = bool)  = bool
	{	E_EQ,	T_INT,		T_INT,		T_BOOL	},	// (int   = int)   = bool
	{	E_EQ,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// (float = float) = bool
	{	E_EQ,	T_INT,		T_FLOAT,	T_BOOL	},	// (int   = float) = bool
	{	E_EQ,	T_FLOAT,	T_INT,		T_BOOL	},	// (float = int)   = bool
	
	// !=
	{	E_NE,	T_BOOL,		T_BOOL,		T_BOOL	},	// (bool  != bool)  = bool
	{	E_NE,	T_INT,		T_INT,		T_BOOL	},	// (int   != int)   = bool
	{	E_NE,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// (float != float) = bool
	{	E_NE,	T_INT,		T_FLOAT,	T_BOOL	},	// (int   != float) = bool
	{	E_NE,	T_FLOAT,	T_INT,		T_BOOL	},	// (float != int)   = bool
	
	// <
	{	E_LT,	T_INT,		T_INT,		T_BOOL	},	// int   < int   = bool
	{	E_LT,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// float < float = bool
	{	E_LT,	T_INT,		T_FLOAT,	T_BOOL	},	// int   < float = bool
	{	E_LT,	T_FLOAT,	T_INT,		T_BOOL	},	// float < int   = bool
	
	// <=
	{	E_LE,	T_INT,		T_INT,		T_BOOL	},	// int   <= int   = bool
	{	E_LE,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// float <= float = bool
	{	E_LE,	T_INT,		T_FLOAT,	T_BOOL	},	// int   <= float = bool
	{	E_LE,	T_FLOAT,	T_INT,		T_BOOL	},	// float <= int   = bool
	
	// >
	{	E_GT,	T_INT,		T_INT,		T_BOOL	},	// int   > int   = bool
	{	E_GT,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// float > float = bool
	{	E_GT,	T_INT,		T_FLOAT,	T_BOOL	},	// int   > float = bool
	{	E_GT,	T_FLOAT,	T_INT,		T_BOOL	},	// float > int   = bool
	
	// >=
	{	E_GE,	T_INT,		T_INT,		T_BOOL	},	// int   >= int   = bool
	{	E_GE,	T_FLOAT,	T_FLOAT,	T_BOOL	},	// float >= float = bool
	{	E_GE,	T_INT,		T_FLOAT,	T_BOOL	},	// int   >= float = bool
	{	E_GE,	T_FLOAT,	T_INT,		T_BOOL	}	// float >= int   = bool
};

static int typesUnary[][3] =
{
	//	op		value		result
	
	// -
	{	E_NEG,	T_INT,		T_INT	},	// -int   = int
	{	E_NEG,	T_FLOAT,	T_FLOAT	},	// -float = float
	
	// !
	{	E_NOT,	T_BOOL,		T_BOOL	}	// !bool = bool
};

static int _convertable[][2] =
{
	//	from		to
	
	{	T_BOOL,		T_BOOL		},
	
	{	T_INT,		T_INT		},
	{	T_INT,		T_FLOAT		},
	
	{	T_FLOAT,	T_FLOAT		},
	{	T_FLOAT,	T_INT		}
};

class OperatorsNames : public map<int, string>
{
public:
	OperatorsNames()
	{
		add(E_SUM,	"operator+");
		add(E_SUB,	"operator-");
		add(E_MUL,	"operator*");
		add(E_POW,	"operator**");
		add(E_DIV,	"operator/");
		add(E_MOD,	"operator%");
		add(E_NEG,	"operator-");

		add(E_AND,	"operator-and");
		add(E_OR,	"operator-or");
		add(E_NOT,	"operator!");

		add(E_EQ,	"operator=");
		add(E_NE,	"operator!=");
		add(E_LT,	"operator<");
		add(E_LE,	"operator<=");
		add(E_GT,	"operator>");
		add(E_GE,	"operator>=");
	}
	
private:
	void add(int id, const string& name)
	{
		insert(std::pair<int, string>(id, name));
	}
};

// TODO: make pointers?
typedef map<string, Func>	FuncMap;

class Analyzer
{
	OperatorsNames opNames;
	
public:
	const Program analyzeProgram(const Program& program)
	{
		const FuncMap& funcs = program.getFuncs();
		FuncMap outFuncs;
		
		FuncMap _funcs;
		
		// original program must contain void main() function
		bool hasMain = false;
		
		// collect information about all functions
		FuncMap::const_iterator func;
		for (func = funcs.begin(); func != funcs.end(); func++)
		{
			// function redefinition
			if (_funcs.find(func->first) != _funcs.end())
			{
				throw AnalyzerError("Function '" + func->first + "' redefinition");
			}
			_funcs[func->first] = func->second;
			
			// int main()
			if (func->second.getType() == T_INT && func->first == "main" && func->second.getArgs().size() == 0)
			{
				hasMain = true;
			}
		}
		
		// oops, no main function
		if (!hasMain)
		{
			// throw AnalyzerError("Function int main() must be defined");
		}
		
		// analyze each function
		for (func = funcs.begin(); func != funcs.end(); func++)
		{
			outFuncs[func->first] = analyzeFunc(func->second, _funcs);
		}
		
		return Program(outFuncs);
	}
	
	const Func analyzeFunc(const Func& func, const FuncMap& funcs)
	{
		int type              = func.getType();
		string name           = func.getName();
		const VarMap& args   = func.getArgs();
		const InstrList& body = func.getBody();
		
		VarMap outArgs;
		InstrList outBody;
		
		VarMap vars;
		
		// analyze function arguments
		VarMap::const_iterator arg;
		for (arg = args.begin(); arg != args.end(); arg++)
		{
			// argument has void type
			if (arg->second == T_VOID)
			{
				throw AnalyzerError(name + ": Argument '" + arg->first + "' has void type");
			}
			
			// argument redeclaration
			if (vars.find(arg->first) != vars.end())
			{
				throw AnalyzerError(name + ": Argument '" + arg->first + "' redeclaration");
			}
			vars[arg->first] = arg->second;
			
			outArgs[arg->first] = arg->second;
		}
		
		// analyze function body if not extern
		if (!func.isExtern())
		{
			outBody = analyzeBody(body, vars, func, funcs);
			return Func(type, name, outArgs, outBody);
		}
		
		// extern function
		return func;
	}
	
	const InstrList analyzeBody(const InstrList& body, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		InstrList outBody;
		
		// analyze each body instruction
		InstrList::const_iterator instr;
		for (instr = body.begin(); instr != body.end(); instr++)
		{
			// analyzed instruction can be empty due optimization
			const Instr *newInstr = analyzeInstr(*instr, vars, func, funcs);
			if (newInstr)
			{
				outBody.push_back(newInstr);
			}
		}
		
		return outBody;
	}
	
	const Instr *analyzeInstr(const Instr *instr, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		switch (instr->getID())
		{
			case I_VAR_DEF:
				return analyzeVarDef(dynamic_cast<const VarDef *>(instr), vars, func, funcs);
			
			case I_VAR_ASSIGN:	  
			case I_VAR_SUM_ASSIGN: case I_VAR_SUB_ASSIGN:
			case I_VAR_MUL_ASSIGN: case I_VAR_POW_ASSIGN:
			case I_VAR_DIV_ASSIGN: case I_VAR_MOD_ASSIGN:
				return analyzeVarAssign(dynamic_cast<const InstrVarAssign *>(instr), vars, func, funcs);
				
			case I_VAR_INC: case I_VAR_DEC:
				return analyzeVarIncDec(dynamic_cast<const InstrVarIncDec *>(instr), vars, func, funcs);
				
			case I_RETURN:
				return analyzeReturn(dynamic_cast<const Return *>(instr), vars, func, funcs);
				
			case I_FUNC_CALL:
				return analyzeIFuncCall(dynamic_cast<const IFuncCall *>(instr), vars, func, funcs);
				
			case I_IF:
				return analyzeIf(dynamic_cast<const If *>(instr), vars, func, funcs);
			
			case I_WHILE:
				return analyzeWhile(dynamic_cast<const While *>(instr), vars, func, funcs);
			
			case I_FOR:
				return analyzeFor(dynamic_cast<const For *>(instr), vars, func, funcs);
			
			case I_PRINT:
				return analyzePrint(dynamic_cast<const Print *>(instr), vars, func, funcs);
		}
	}
	
	const VarDef *analyzeVarDef(const VarDef *instr, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type           = instr->getType();
		const string& name = instr->getName();
		const Expr *value  = instr->getValue();
		
		// variable redefinition
		if (vars.find(name) != vars.end())
		{
			throw AnalyzerError(func.getName() + ": Variable '" + name + "' redefinition");
		}
		vars[name] = instr->getType();
		
		// type mismatch
		if (value)
		{
			int valueType = typeExpr(value, vars, func, funcs);
			if (!convertable(valueType, type))
			{
				throw AnalyzerError(func.getName() + ": Variable '" + name + "' assignment, inconvertable types, " + typeName(valueType) + " given, " + typeName(type) + " expected");
			}
		}
		
		return instr;
	}
	
	const InstrVarAssign *analyzeVarAssign(const InstrVarAssign *instr, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const string& name = instr->getName();
		const Expr *value  = instr->getValue();
		
		// undeclared variable
		VarMap::const_iterator i = vars.find(name);
		if (i == vars.end())
		{
			throw AnalyzerError(func.getName() + ": Undeclared variable '" + name + "'");
		}
		
		// type mismatch
		int type = i->second;
		int valueType = typeExpr(value, vars, func, funcs);
		if (!convertable(valueType, type))
		{
			throw AnalyzerError(func.getName() + ": Variable '" + name + "' assignment, inconvertable types, " + typeName(valueType) + " given, " + typeName(type) + " expected");
		}
		
		return instr;
	}
	
	const InstrVarIncDec *analyzeVarIncDec(const InstrVarIncDec *instr, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const string& name = instr->getName();
		
		// undeclared variable
		VarMap::const_iterator i = vars.find(name);
		if (i == vars.end())
		{
			throw AnalyzerError(func.getName() + ": Undeclared variable '" + name + "'");
		}
		
		// bool++ or bool-- is nonsence
		if (i->second == T_BOOL)
		{
			string action = (instr->getID() == I_VAR_INC ? "Increment" : "Decrement");
			throw AnalyzerError(func.getName() + ": " + action + " for boolean variable '" + name + "'");
		}
		// float++ or float-- is nonsence
		if (i->second == T_FLOAT)
		{
			string action = (instr->getID() == I_VAR_INC ? "Increment" : "Decrement");
			throw AnalyzerError(func.getName() + ": " + action + " for float variable '" + name + "'");
		}
		
		return instr;
	}
	
	const Return *analyzeReturn(const Return *instr, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type = func.getType();
		const Expr *value = instr->getValue();
		
		// void function returns value
		if (type == T_VOID && value)
		{
			throw AnalyzerError(func.getName() + ": Returning not null value for void function");
		}
		
		// type mismatch
		int returnType = typeExpr(value, vars, func, funcs);
		if (!convertable(returnType, type))
		{
			throw AnalyzerError(func.getName() + ": Return, inconvertable types " + typeName(returnType) + " given, " + typeName(type) + " expected");
		}
		
		return instr;
	}
	
	const IFuncCall *analyzeIFuncCall(const IFuncCall *instr, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const string& name   = instr->getName();
		const ExprList& args = instr->getArgs();
		
		analyzeFuncCall(name, args, vars, func, funcs);
		
		return instr;
	}
	
	const If *analyzeIf(const If *instr, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const Expr *cond    = instr->getCond();;
		// recursively analyze both branches
		InstrList trueBody  = analyzeBody(instr->getTrueBody(),  vars, func, funcs);
		InstrList falseBody = analyzeBody(instr->getFalseBody(), vars, func, funcs);
		
		// validate condition and check its type
		int condType = typeExpr(cond, vars, func, funcs);
		if (!convertable(condType, T_BOOL))
		{
			throw AnalyzerError(func.getName() + ": Boolean condition expected to if, " + typeName(condType) + " given");
		}
		
		// ignore empty if
		if (trueBody.size() == 0 && falseBody.size() == 0)
		{
			return 0;
		}
		
		// if has only false branch we should better reverse it
		if (trueBody.size() == 0)
		{
			return new If(new Not(cond), falseBody, trueBody);
		}
		
		return new If(cond, trueBody, falseBody);
	}
	
	const While *analyzeWhile(const While *instr, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const Expr *cond = instr->getCond();
		// recursively analyze body
		InstrList body   = analyzeBody(instr->getBody(), vars, func, funcs);
		
		// validate condition and check its type
		int condType = typeExpr(cond, vars, func, funcs);
		if (!convertable(condType, T_BOOL))
		{
			throw AnalyzerError(func.getName() + ": Boolean condition expected to while, " + typeName(condType) + " given");
		}
		
		return new While(cond, body);
	}
	
	const For *analyzeFor(const For *instr, VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		// recursively analyze initialization
		InstrList init   = analyzeBody(instr->getInit(), vars, func, funcs);
		const Expr *cond = instr->getCond();
		// recursively analyze stepping
		InstrList step   = analyzeBody(instr->getStep(), vars, func, funcs);
		InstrList body   = analyzeBody(instr->getBody(), vars, func, funcs);
		
		// condition may be empty, and it's equivalent to true
		if (!cond)
		{
			cond = new Bool(true);
		}
		
		// validate condition and check its type
		int condType = typeExpr(cond, vars, func, funcs);
		if (!convertable(condType, T_BOOL))
		{
			throw AnalyzerError(func.getName() + ": Boolean condition expected to for, " + typeName(condType) + " given");
		}
		
		return new For(init, cond, step, body);
	}
	
	const Print *analyzePrint(const Print *instr, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		// zero parameters, it's not good
		if (instr->getArgs().size() == 0)
		{
			throw AnalyzerError(func.getName() + ": No arguments given to print");
		}
		
		// validate each argument
		const ExprList& args = instr->getArgs();
		ExprList::const_iterator i;
		for (i = args.begin(); i != args.end(); i++)
		{
			typeExpr(*i, vars, func, funcs);
		}
		
		return instr;
	}
	
	int typeExpr(const Expr *e, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		switch (e->getID())
		{
			case E_VAR:
				return typeVar(dynamic_cast<const Var *>(e), vars, func, funcs);
			
			case E_BOOL:
				return T_BOOL;
			case E_INT:
				return T_INT;
			case E_FLOAT:
				return T_FLOAT;
			case E_STRING:
				return T_STRING;
			
			case E_SUM: case E_SUB:
			case E_MUL: case E_POW:
			case E_DIV: case E_MOD:
				return typeBinary(dynamic_cast<const ExprBinary *>(e), vars, func, funcs);
			case E_NEG:
				return typeUnary(dynamic_cast<const ExprUnary *>(e), vars, func, funcs);
			
			case E_AND: case E_OR:
				return typeBinary(dynamic_cast<const ExprBinary *>(e),  vars, func, funcs);
			case E_NOT:
				return typeUnary(dynamic_cast<const ExprUnary *>(e), vars, func, funcs);
			
			case E_EQ: case E_NE:
			case E_LT: case E_LE:
			case E_GT: case E_GE:
				return typeBinary(dynamic_cast<const ExprBinary *>(e), vars, func, funcs);
			
			case E_FUNC_CALL:
				return typeFuncCall(dynamic_cast<const EFuncCall *>(e), vars, func, funcs);
		}
	}
	
	int typeVar(const Var *e, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const string& name = e->getValue();
		
		// undeclared variable
		VarMap::const_iterator i = vars.find(name);
		if (i == vars.end())
		{
			throw AnalyzerError(func.getName() + ": Undeclared variable '" + name + "'");
		}
		
		return i->second;
	}
	
	// TODO: use convertable?
	int typeBinary(const ExprBinary *e, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int rules = sizeof(typesBinary) / sizeof(typesBinary[0]);
		
		int type1 = typeExpr(e->getFirst(),  vars, func, funcs);
		int type2 = typeExpr(e->getSecond(), vars, func, funcs);
		
		for (int i = 0; i < rules; i++)
		{
			if (typesBinary[i][0] == e->getID()
			 && typesBinary[i][1] == type1
			 && typesBinary[i][2] == type2)
			{
				return typesBinary[i][3];
			}
		}
		
		throw AnalyzerError(func.getName() + ": Invalid arguments types for " + opNames[e->getID()] + "(" + typeName(type1) + ", " + typeName(type2) + ")");
	}
	
	int typeUnary(const ExprUnary *e, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int rules = sizeof(typesUnary) / sizeof(typesUnary[0]);
		
		int type1 = typeExpr(e->getValue(), vars, func, funcs);
		
		for (int i = 0; i < rules; i++)
		{
			if (typesUnary[i][0] == e->getID()
			 && typesUnary[i][1] == type1)
			{
				return typesBinary[i][2];
			}
		}
		
		throw AnalyzerError(func.getName() + ": Invalid argument type for " + opNames[e->getID()] + "(" + typeName(type1) + ")");
	}
	
	int typeFuncCall(const EFuncCall *e, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const string& name   = e->getName();
		const ExprList& args = e->getArgs();
		
		analyzeFuncCall(name, args, vars, func, funcs);
		
		return funcs.find(name)->second.getType();
	}
	
	void analyzeFuncCall(const string& name, const ExprList& args, const VarMap& vars, const Func& func, const FuncMap& funcs)
	{
		// call ot undefined function
		FuncMap::const_iterator i = funcs.find(name);
		if (i == funcs.end())
		{
			throw AnalyzerError(func.getName() + ": Call to undefined function '" + name + "'");
		}
		
		// wrong arguments count
		const Func& funcDef        = i->second;
		const VarMap& funcDefArgs = funcDef.getArgs();
		if (funcDefArgs.size() != args.size())
		{
			// FIXME: what the fuck
			std::stringstream s1, s2;
			s1 << funcDefArgs.size();
			s2 << args.size();
			string expected = s1.str();
			string given    = s2.str();
			throw AnalyzerError(func.getName() + ": Function call '" + name + "', wrong arguments count, " + given + " given, " + expected + " expected");
		}
		
		// validate each argument and check types
		VarMap::const_iterator   declared;
		ExprList::const_iterator given;
		for (declared  = funcDefArgs.begin(), given  = args.begin();
		     declared != funcDefArgs.end() && given != args.end();
		     declared++, given++)
		{
			int givenType = typeExpr(*given, vars, func, funcs);
			if (!convertable(givenType, declared->second))
			{
				throw AnalyzerError(func.getName() + ": Function call '" + name + "', argument '" + declared->first + "', inconvertable types, " + typeName(givenType) + " given, " + typeName(declared->second) + " expected");
			}
		}
	}
	
	const string typeName(int type)
	{
		switch (type)
		{
			case T_VOID:	return "void";
			case T_BOOL:	return "boolean";
			case T_INT:		return "integer";
			case T_FLOAT:	return "float";
			case T_STRING:	return "string";
			default:		throw AnalyzerError("Unknown type " + type);
		}
	}
	
	bool convertable(int from, int to)
	{
		static const int rows = sizeof(_convertable) / sizeof(_convertable[0]);
		for (int i = 0; i < rows; i++)
		{
			if (_convertable[i][0] == from
			 && _convertable[i][1] == to)
			{
				return true;
			}
		}
		
		return false;
	}
};