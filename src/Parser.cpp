#include "Parser.h"

#include <iostream>

Parser::Parser() : lex(NULL) {}

Parser::Parser(Lex *lex) : lex(lex)
{
	token = lex->next();
}

void Parser::setLex(Lex *lex)
{
	this->lex = lex;
	token = lex->next();
}

const Program Parser::parse()
{
	if (!lex)
	{
		throw ParserError("Lexer is not set", 0, 0);
	}
	
	FuncMap funcs;
	parseProgram(funcs);
	return Program(funcs);
}

#define pass(x) _pass(x, #x)

inline void Parser::_pass(int type, const char *tokenName)
{
	if (token != type)
	{
		throw ParserError(string(tokenName) + " expected but got " + token.getName(), lex->getLine(), lex->getCol());
	}
	token = lex->next();
}

void Parser::parseProgram(FuncMap& funcs)
{
	/* program -> functions EOF */
	
	parseFuncs(funcs);	// functions
	pass(T_EOF);		// EOF
}

void Parser::parseFuncs(FuncMap& funcs)
{
	/* functions -> <E> */
	
	if (token == T_EOF)
	{
		return;
	}
	
	/* functions -> function functions */
	
	parseFunc(funcs);	// function
	parseFuncs(funcs);	// functions
}

inline bool Parser::isType()
{
	return token == T_VOID
	    || token == T_BOOL
	    || token == T_INT
	    || token == T_FLOAT;
}

int Parser::parseType()
{
	/* type -> VOID | BOOL | INT | FLOAT */
	
	if (!isType())
	{
		throw ParserError("Type expected", lex->getLine(), lex->getCol());
	}
	int type = token.getType();
	pass(type);
	return type;
}

void Parser::parseFunc(FuncMap& funcs)
{
	int type;
	string name;
	VarMap args;
	InstrList body;
	
	/* function -> EXTERN type IDENTIFIER ( arguments ) ; */
	
	if (token == T_EXTERN)
	{
		pass(T_EXTERN);			// EXTERN
		type = parseType();		// type
		name = token.getData();
		pass(T_IDENTIFIER);		// IDENTIFIER
		pass(T_LEFT);			// (
		parseArgs(args);		// arguments
		pass(T_RIGHT);			// )
		pass(T_SEMICOLON);		// ;

		funcs[name] = Func(type, name, args);
	}

	/* function -> type IDENTIFIER ( arguments ) body */
	
	else
	{
		type = parseType();		// type
		name = token.getData();
		pass(T_IDENTIFIER);		// IDENTIFIER
		pass(T_LEFT);			// (
		parseArgs(args);		// arguments
		pass(T_RIGHT);			// )
		parseBody(body);		// body
		
		funcs[name] = Func(type, name, args, body);
	}
}

void Parser::parseArgs(VarMap& args)
{
	/* arguments -> <E> */
	
	if (!isType())
	{
		return;
	}
	
	/* arguments -> argument arguments' */
	
	parseArg(args);		// argument
	parseArgs_(args);	// arguments'
}

void Parser::parseArgs_(VarMap& args)
{
	/* arguments' -> <E> */
	
	if (token != T_COMMA)
	{
		return;
	}
	
	/* arguments' -> , argument arguments' */
	
	pass(T_COMMA);		// ,
	parseArg(args);		// argument
	parseArgs_(args);	// arguments'
}

void Parser::parseArg(VarMap& args)
{
	int type;
	string name;
	
	/* argument -> type IDENTIFIER */
	
	type = parseType();		// type
	name = token.getData();
	pass(T_IDENTIFIER);		// IDENTIFIER
	
	args[name] = type;
}

void Parser::parseBody(InstrList& instrs)
{
	/* body -> instruction */
	
	if (token != T_LEFT_CURLY)
	{
		parseInstr(instrs);
		return;
	}
	
	/* body -> block */
	
	parseBlock(instrs);
}

void Parser::parseBlock(InstrList& instrs)
{
	/* block -> { list } */
	
	pass(T_LEFT_CURLY);		// {
	parseList(instrs);		// list
	pass(T_RIGHT_CURLY);	// }
}

void Parser::parseList(InstrList& instrs)
{
	/* list -> <E> */
	
	if (token == T_RIGHT_CURLY)
	{
		return;
	}
	
	/* list -> instruction list */
	
	parseInstr(instrs);	// instruction
	parseList(instrs);	// list
}

void Parser::parseInstr(InstrList& instrs)
{
	/* instruction -> ; */
	
	if (token == T_SEMICOLON)
	{
		pass(T_SEMICOLON); // ;
		return;
	}
	
	/* instruction -> definitions ; */
	
	if (isType())
	{
		parseDefinitions(instrs);	// definitions
		pass(T_SEMICOLON);			// ;
		return;
	}
	
	/* instruction -> assignment_or_function_call ; */
	
	if (token == T_IDENTIFIER)
	{
		parseAssignOrFuncCall(instrs);	// assignment_or_function_call
		pass(T_SEMICOLON);				// ;
		return;
	}
	
	/* instruction -> return */
	
	if (token == T_RETURN)
	{
		parseReturn(instrs);	// return
		pass(T_SEMICOLON);		// ;
		return;
	}
	
	/* instruction -> if */
	
	if (token == T_IF)
	{
		parseIf(instrs);		// if
		return;
	}
	
	/* instruction -> while */
	
	if (token == T_WHILE)
	{
		parseWhile(instrs);		// while
		return;
	}
	
	/* instruction -> for */
	
	if (token == T_FOR)
	{
		parseFor(instrs);		// for
		return;
	}
	
	/* instruction -> print ; */
	
	if (token == T_PRINT)
	{
		parsePrint(instrs);		// print
		pass(T_SEMICOLON);		// ;
		return;
	}
	
	throw ParserError("Instruction expected", lex->getLine(), lex->getCol());
}

void Parser::parseDefinitions(InstrList& instrs)
{
	int type;
	
	/* definitions -> type definition definitions' */
	
	type = parseType();					// type
	parseDefinition(instrs, type);		// definition
	parseDefinitions_(instrs, type);	// definitions'
}

void Parser::parseDefinitions_(InstrList& instrs, int type)
{
	/* definitions' -> <E> */
	
	if (token != T_COMMA)
	{
		return;
	}
	
	/* definitions' -> , definition definitions' */
	
	pass(T_COMMA);						// ,
	parseDefinition(instrs, type);		// definition
	parseDefinitions_(instrs, type);	// definitions'
}

void Parser::parseDefinition(InstrList& instrs, int type)
{
	string name;
	const Expr *value = 0;
	
	/* definition -> IDENTIFIER */
	
	name = token.getData();
	pass(T_IDENTIFIER);	// IDENTIFIER
	
	/* definition -> IDENTIFIER = expression */
	
	if (token == T_EQ)
	{
		pass(T_EQ);			// =
		value = parseExpr();	// expression
	}
	
	instrs.push_back(new VarDef(type, name, value));
}

void Parser::parseAssignOrFuncCall(InstrList& instrs)
{
	string name;
	
	name = token.getData();
	pass(T_IDENTIFIER);		// IDENTIFIER
	
	/* assignment_or_function_call -> IDENTIFIER = expression */
	
	if (token == T_EQ)
	{
		const Expr *value;
		
		pass(T_EQ);			// =
		value = parseExpr();	// expression
		
		instrs.push_back(new VarAssign(name, value));
		return;
	}
	
	/* assignment_or_function_call -> IDENTIFIER += expression */
	
	/*if (token == T_SUM_ASSIGN)
	{
		const Expr *value;
		
		pass(T_SUM_ASSIGN);		// +=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarSumAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER -= expression */
	
	/*if (token == T_SUB_ASSIGN)
	{
		const Expr *value;
		
		pass(T_SUB_ASSIGN);		// -=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarSubAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER *= expression */
	
	/*if (token == T_MUL_ASSIGN)
	{
		const Expr *value;
		
		pass(T_MUL_ASSIGN);		// *=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarMulAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER **= expression */
	
	/*if (token == T_POW_ASSIGN)
	{
		const Expr *value;
		
		pass(T_POW_ASSIGN);		// **=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarPowAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER /= expression */
	
	/*if (token == T_DIV_ASSIGN)
	{
		const Expr *value;
		
		pass(T_DIV_ASSIGN);		// /=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarDivAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER %= expression */
	
	/*if (token == T_MOD_ASSIGN)
	{
		const Expr *value;
		
		pass(T_MOD_ASSIGN);		// %=
		value = parseExpr();	// expression
		
		instrs.push_back(new VarModAssign(name, value));
		return;
	}*/
	
	/* assignment_or_function_call -> IDENTIFIER ++ */
	
	if (token == T_INC)
	{
		pass(T_INC); // ++
		
		instrs.push_back(new VarInc(name));
		return;
	}
	
	/* assignment_or_function_call -> IDENTIFIER -- */
	
	if (token == T_DEC)
	{
		pass(T_DEC); // --
		
		instrs.push_back(new VarDec(name));
		return;
	}
	
	/* assignment_or_function_call -> IDENTIFIER ( expressions ) */
	
	if (token == T_LEFT)
	{
		ExprList exprs;
		
		pass(T_LEFT);		// (
		parseExprs(exprs);	// expressions
		pass(T_RIGHT);		// )
		
		instrs.push_back(new IFuncCall(name, exprs));
		return;
	}
	
	throw ParserError("Unexpected identifier", lex->getLine(), lex->getCol());
}

void Parser::parseExprs(ExprList& exprs)
{
	/* expressions -> <E> */
	
	if (token == T_RIGHT)
	{
		return;
	}
	
	const Expr *expr;
	
	/* expressions -> expression expressions' */
	
	expr = parseExpr();		// expression
	exprs.push_back(expr);
	parseExprs_(exprs);		// expressions'
}

void Parser::parseExprs_(ExprList& exprs)
{
	/* expressions' -> <E> */
	
	if (token != T_COMMA)
	{
		return;
	}

	const Expr *expr;
	
	/* expressions' -> , expression expressions' */

	pass(T_COMMA);			// ,
	expr = parseExpr();		// expression
	exprs.push_back(expr);
	parseExprs_(exprs);		// expressions'
}

void Parser::parseReturn(InstrList& instrs)
{
	const Expr *value = 0;
	
	/* return -> RETURN */
	
	pass(T_RETURN); // RETURN
	
	/* return -> RETURN expression */
	
	if (token != T_SEMICOLON)
	{
		value = parseExpr();
	}
	
	instrs.push_back(new Return(value));
}

void Parser::parseIf(InstrList& instrs)
{
	const Expr *cond;
	InstrList trueBody;
	InstrList falseBody;
	
	/* if -> IF ( expression ) body */
	
	pass(T_IF);				// IF
	pass(T_LEFT);			// (
	cond = parseExpr();		// expression
	pass(T_RIGHT);			// )
	parseBody(trueBody);	// body
	
	/* if -> IF ( expression ) body ELSE body */
	
	if (token == T_ELSE)
	{
		pass(T_ELSE);			// ELSE
		parseBody(falseBody);	// body
	}
	
	instrs.push_back(new If(cond, trueBody, falseBody));
}

void Parser::parseWhile(InstrList& instrs)
{
	const Expr *cond;
	InstrList body;
	
	/* while -> WHILE ( expression ) body */
	
	pass(T_WHILE);		// WHILE
	pass(T_LEFT);		// (
	cond = parseExpr();	// expression
	pass(T_RIGHT);		// )
	parseBody(body);	// body
	
	instrs.push_back(new While(cond, body));
}

void Parser::parseFor(InstrList& instrs)
{
	InstrList init;
	const Expr *cond;
	InstrList step;
	InstrList body;
	
	/* for -> FOR ( body for_cond body ) body */
	
	pass(T_FOR);			// FOR
	pass(T_LEFT);			// (
	parseBody(init);		// body
	cond = parseForCond();	// for_cond
	parseBody(step);		// body
	pass(T_RIGHT);			// )
	parseBody(body);		// body
	
	instrs.push_back(new For(init, cond, step, body));
}

const Expr *Parser::parseForCond()
{
	/* for_cond -> ; */
	
	if (token == T_SEMICOLON)
	{
		pass(T_SEMICOLON); // ;
		return 0;
	}
	
	const Expr *cond;
	
	/* for_cond -> expression ; */
	
	cond = parseExpr();	// expression
	pass(T_SEMICOLON);	// ;
	
	return cond;
}

void Parser::parsePrint(InstrList& instrs)
{
	ExprList exprs;
	
	/* print -> PRINT ( expressions ) */
	
	pass(T_PRINT);		// PRINT
	pass(T_LEFT);		// (
	parseExprs(exprs);	// expressions
	pass(T_RIGHT);		// )
	
	instrs.push_back(new Print(exprs));
}

const Expr *Parser::parseExpr()
{
	return parseDisjunction();
}

const Expr *Parser::parseDisjunction()
{
	const Expr *last;
	const Expr *next;
	
	/* disjunction -> conjunction OR conjunction OR ... */
	
	last = parseConjunction(); // conjunction
	
	while (token == T_OR)
	{
		pass(T_OR);					// OR
		next = parseConjunction();	// conjunction
		
		last = new Or(last, next);
	}
	
	return last;
}

const Expr *Parser::parseConjunction()
{
	const Expr *last;
	const Expr *next;
	
	/* conjunction -> equality OR equality OR ... */
	
	last = parseEquality(); // equality
	
	while (token == T_AND)
	{
		pass(T_AND);			// AND
		next = parseEquality();	// equality
		
		last = new And(last, next);
	}
	
	return last;
}

const Expr *Parser::parseEquality()
{
	const Expr *last;
	const Expr *next;
	
	/* equality_operator -> = | != */
	/* equality -> relation equality_operator relation equality_operator ... */
	
	last = parseRelation(); // relation
	
	while (token == T_EQ || token == T_NE)
	{
		int type = token.getType();
		
		pass(type);				// equality_operator
		next = parseRelation();	// relation
		
		if (type == T_EQ)	last = new Eq(last, next);
		if (type == T_NE)	last = new Ne(last, next);
	}
	
	return last;
}

const Expr *Parser::parseRelation()
{
	const Expr *last;
	const Expr *next;

	/* relation_operator -> < | <= | > | >= */
	/* relation -> addition relation_operator addition relation_operator ... */
	
	last = parseAddition();
	
	while (token == T_LT || token == T_LE
	    || token == T_GT || token == T_GE)
	{
		int type = token.getType();
		
		pass(type);				// relation_operator
		next = parseAddition();	// addition
		
		if (type == T_LT)	last = new Lt(last, next);
		if (type == T_LE)	last = new Le(last, next);
		if (type == T_GT)	last = new Gt(last, next);
		if (type == T_GE)	last = new Ge(last, next);
	}
	
	return last;
}

const Expr *Parser::parseAddition()
{
	const Expr *last;
	const Expr *next;

	/* addition_operator -> + | - */
	/* addition -> multiplication addition_operator multiplication addition_operator ... */
	
	last = parseMultiplication();
	
	while (token == T_PLUS || token == T_MINUS)
	{
		int type = token.getType();
		
		pass(type);						// addition_operator
		next = parseMultiplication();	// multiplication
		
		if (type == T_PLUS)		last = new Sum(last, next);
		if (type == T_MINUS)	last = new Sub(last, next);
	}
	
	return last;
}

const Expr *Parser::parseMultiplication()
{
	const Expr *last;
	const Expr *next;
	
	/* multiplication_operator -> * | / | % */
	/* multiplication -> exponentiation multiplication_operator exponentiation multiplication_operator ... */
	
	//last = parseExponentiation();
	last = parseUnary();
	
	while (token == T_MUL || token == T_DIV || token == T_MOD)
	{
		int type = token.getType();
		
		pass(type);						// multiplication_operator
		//next = parseExponentiation();	// exponentiation
		next = parseUnary();
		
		if (type == T_MUL) last = new Mul(last, next);
		if (type == T_DIV) last = new Div(last, next);
		if (type == T_MOD) last = new Mod(last, next);
	}
	
	return last;
}

const Expr *Parser::parseExponentiation()
{
	const Expr *unary;
	const Expr *expo = 0;
	
	/* exponentiation -> unary */
	
	unary = parseUnary();
	
	/* exponentiation -> unary ** exponentiation */
	
	if (token == T_POW)
	{
		pass(T_POW);					// **
		expo = parseExponentiation();	// exponentiation
	}
	
	return !expo ? unary : new Pow(unary, expo);
}

const Expr *Parser::parseUnary()
{
	/* unary -> + unary */
	
	if (token == T_PLUS)
	{
		pass(T_PLUS);					// +
		return parseUnary();			// unary
	}
	
	/* unary -> - unary */
	
	if (token == T_MINUS)
	{
		pass(T_MINUS);					// -
		return new Neg(parseUnary());	// unary
	}
	
	/* unary -> ! unary */
	
	if (token == T_NOT)
	{
		pass(T_NOT);					// !
		return new Not(parseUnary());	// unary
	}
	
	/* unary -> atom */
	
	return parseAtom(); // atom
}

const Expr *Parser::parseAtom()
{
	string data;
	
	/* atom -> BOOLEAN_CONSTANT */
	
	if (token == T_TRUE || token == T_FALSE)
	{
		int type = token.getType();
		pass(type);							// BOOLEAN_CONSTANT
		return new Bool(type == T_TRUE);
	}
	
	/* atom -> INTEGER_CONSTANT */
	
	if (token == T_INTEGER_CONSTANT)
	{
		data = token.getData();
		pass(T_INTEGER_CONSTANT);			// INTEGER_CONSTANT
		return new Int(strtol(data.c_str(), 0, 10));
	}
	
	/* atom -> FLOAT_CONSTANT */
	
	if (token == T_FLOAT_CONSTANT)
	{
		data = token.getData();
		pass(T_FLOAT_CONSTANT);				// FLOAT_CONSTANT
		return new Float(strtof(data.c_str(), 0));
	}
	
	/* atom -> STRING_LITERAL */
	
	if (token == T_STRING)
	{
		data = token.getData();
		pass(T_STRING);				// STRING_LITERAL
		return new String(data);
	}
	
	/* atom -> ( expression ) */
	
	if (token == T_LEFT)
	{
		const Expr *expr;
		
		pass(T_LEFT);		// (
		expr = parseExpr();	// expression
		pass(T_RIGHT);		// )
		
		return expr;
	}
	
	/* atom -> IDENTIFIER */
	
	if (token == T_IDENTIFIER)
	{
		string name;
		
		name = token.getData();
		pass(T_IDENTIFIER);		// IDENTIFIER
		
		if (token == T_LEFT)
		{
			ExprList args;
			
			/* atom -> IDENTIFIER ( expressions ) */
			
			pass(T_LEFT);		// (
			parseExprs(args);	// expressions
			pass(T_RIGHT);		// )
			
			return new EFuncCall(name, args);
		}
		else
		{
			return new Var(name);
		}
	}
	
	throw ParserError("Expression expected", lex->getLine(), lex->getCol());
}