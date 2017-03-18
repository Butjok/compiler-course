#include "XML.h"

void XML::printProgram(const Program& program)
{
	FuncMap::const_iterator i;
	const FuncMap& funcs = program.getFuncs();
	
	cout << "<program>" << endl;
	for (i = funcs.begin(); i != funcs.end(); i++)
	{
		printFunc(i->second);
	}
	cout << "</program>" << endl;
}

void XML::printFunc(const Func& func)
{
	string type = this->typeName(func.getType());
	string name = func.getName();
	const VarMap& args = func.getArgs();
	const InstrList& body = func.getBody();
	
	cout << "\t<function type='" << type << "' name='" << name << "'>" << endl;
	
	if (args.empty())
	{
		cout << "\t\t<arguments />" << endl;
	}
	else
	{
		VarMap::const_iterator i;
		
		cout << "\t\t<arguments>" << endl;
		for (i = args.begin(); i != args.end(); i++)
		{
			string type = this->typeName(i->second);
			string name = i->first;
			
			cout << "\t\t\t<argument type='" << type << "' name='" << name << "' />" << endl;
		}
		cout << "\t\t</arguments>" << endl;
	}
	
	if (body.empty())
	{
		cout << "\t\t<body />" << endl;
	}
	else
	{
		cout << "\t\t<body>" << endl;
		printInstrs(body, 3);
		cout << "\t\t</body>" << endl;
	}
	
	cout << "\t</function>" << endl;
}

void XML::printInstrs(const InstrList& instrs, int t)
{
	InstrList::const_iterator i;
	
	for (i = instrs.begin(); i != instrs.end(); i++)
	{
		printInstr(*i, t);
	}
}

void XML::printInstr(const Instr *i, int t)
{
	switch (i->getID())
	{
		case I_VAR_DEF:
			printVarDef(dynamic_cast<const VarDef* >(i), t);			break;
		
		case I_VAR_ASSIGN:
		case I_VAR_SUM_ASSIGN: case I_VAR_SUB_ASSIGN:
		case I_VAR_MUL_ASSIGN: case I_VAR_POW_ASSIGN:
		case I_VAR_DIV_ASSIGN: case I_VAR_MOD_ASSIGN:
			printVarAssign(dynamic_cast<const InstrVarAssign *>(i), t);	break;
		
		case I_VAR_INC:
		case I_VAR_DEC:
			printVarIncDec(dynamic_cast<const InstrVarIncDec *>(i), t);	break;
		
		case I_RETURN:
			printReturn(dynamic_cast<const Return *>(i), t);			break;
		
		case I_FUNC_CALL:
			printIFuncCall(dynamic_cast<const IFuncCall *>(i), t);		break;
		
		case I_IF:
			printIf(dynamic_cast<const If *>(i), t);					break;
		
		case I_WHILE:
			printWhile(dynamic_cast<const While *>(i), t);				break;
		case I_FOR:
			printFor(dynamic_cast<const For *>(i), t);					break;
		
		case I_PRINT:
			printPrint(dynamic_cast<const Print *>(i), t);				break;
	}
}

void XML::printVarDef(const VarDef *i, int t)
{
	string type = this->typeName(i->getType());
	string name = i->getName();
	const Expr *value = i->getValue();
	
	if (value)
	{
		tabs(t); cout << "<variable type='" << type << "' name='" << name << "'>" << endl;
		printExpr(value, t + 1);
		tabs(t); cout << "</variable>" << endl;
	}
	else
	{
		tabs(t); cout << "<variable type='" << type << "' name='" << name << "' />" << endl;
	}
}

void XML::printVarAssign(const InstrVarAssign *i, int t)
{
	const string& instrName = tagsInstrs[i->getID()];
	
	const string& name = i->getName();
	const Expr *value  = i->getValue();
	
	tabs(t); cout << "<" << instrName << " name='" << name << "'>" << endl;
	printExpr(value, t + 1);
	tabs(t); cout << "</" << instrName << ">" << endl;
}

void XML::printVarIncDec(const InstrVarIncDec *i, int t)
{
	const string& instrName = tagsInstrs[i->getID()];
	
	const string& name = i->getName();
	
	tabs(t); cout << "<" << instrName << " name='" << name << "' />" << endl;
}

void XML::printReturn(const Return *i, int t)
{
	const Expr *value = i->getValue();
	
	tabs(t); cout << "<return>" << endl;
	printExpr(value, t + 1);
	tabs(t); cout << "</return>" << endl;
}

void XML::printIFuncCall(const IFuncCall *i, int t)
{
	const string& name   = i->getName();
	const ExprList& args = i->getArgs();
	
	if (args.empty())
	{
		tabs(t); cout << "<function-call name='" << name << "' />" << endl;
	}
	else
	{
		tabs(t); cout << "<function-call name='" << name << "'>" << endl;
		
		ExprList::const_iterator ii;
		for (ii = args.begin(); ii != args.end(); ii++)
		{
			tabs(t); cout << "\t<argument>" << endl;
			printExpr(*ii, t + 2);
			tabs(t); cout << "\t</argument>" << endl;
		}
		
		tabs(t); cout << "</function-call>" << endl;
	}
}

void XML::printIf(const If *i, int t)
{
	const Expr *cond = i->getCond();
	const InstrList& trueBody  = i->getTrueBody();
	const InstrList& falseBody = i->getFalseBody();
	
	tabs(t); cout << "<if>" << endl;
	
	tabs(t); cout << "\t<condition>" << endl;
	printExpr(cond, t + 2);
	tabs(t); cout << "\t</condition>" << endl;
	
	if (trueBody.empty())
	{
		tabs(t); cout << "\t<true-body />" << endl;
	}
	else
	{
		tabs(t); cout << "\t<true-body>" << endl;
		printInstrs(trueBody, t + 2);
		tabs(t); cout << "\t</true-body>" << endl;
	}
	
	if (falseBody.empty())
	{
		tabs(t); cout << "\t<false-body />" << endl;
	}
	else
	{
		tabs(t); cout << "\t<false-body>" << endl;
		printInstrs(falseBody, t + 2);
		tabs(t); cout << "\t</false-body>" << endl;
	}
	
	tabs(t); cout << "</if>" << endl;
}

void XML::printWhile(const While *i, int t)
{
	const Expr *cond = i->getCond();
	const InstrList& body = i->getBody();
	
	tabs(t); cout << "<while>" << endl;
	tabs(t); cout << "\t<condition>" << endl;
	printExpr(cond, t + 2);
	tabs(t); cout << "\t</condition>" << endl;
	
	if (body.empty())
	{
		tabs(t); cout << "\t<body />" << endl;
	}
	else
	{
		tabs(t); cout << "\t<body>" << endl;
		printInstrs(body, t + 2);
		tabs(t); cout << "\t</body>" << endl;
	}
	
	tabs(t); cout << "</while>" << endl;
}

void XML::printFor(const For *i, int t)
{
	const InstrList& init = i->getInit();
	const Expr *cond = i->getCond();
	const InstrList& step = i->getStep();
	
	tabs(t); cout << "<for>" << endl;
	
	if (init.empty())
	{
		tabs(t); cout << "\t<initialization />" << endl;
	}
	else
	{
		tabs(t); cout << "\t<initialization>" << endl;
		printInstrs(init, t + 2);
		tabs(t); cout << "\t</initialization>" << endl;
	}
	
	tabs(t); cout << "\t<condition>" << endl;
	printExpr(cond, t + 2);
	tabs(t); cout << "\t</condition>" << endl;
	
	if (step.empty())
	{
		tabs(t); cout << "\t<stepping />" << endl;
	}
	else
	{
		tabs(t); cout << "\t<stepping>" << endl;
		printInstrs(step, t + 2);
		tabs(t); cout << "\t</stepping>" << endl;
	}
	
	tabs(t); cout << "</for>" << endl;
}

void XML::printPrint(const Print *i, int t)
{
	const ExprList& exprs = i->getArgs();
	
	if (exprs.empty())
	{
		tabs(t); cout << "<print />" << endl;
	}
	else
	{
		tabs(t); cout << "<print>" << endl;
		
		ExprList::const_iterator ii;
		for (ii = exprs.begin(); ii != exprs.end(); ii++)
		{
			tabs(t); cout << "\t<argument>" << endl;
			printExpr(*ii, t + 2);
			tabs(t); cout << "\t</argument>" << endl;
		}
		
		tabs(t); cout << "</print>" << endl;
	}
}

void XML::printExpr(const Expr *e, int t)
{
	switch (e->getID())
	{
		case E_VAR:
			printVar(dynamic_cast<const Var *>(e), t);				break;
		
		case E_BOOL:
			printBool(dynamic_cast<const Bool *>(e), t);			break;
		case E_INT:
			printInt(dynamic_cast<const Int *>(e), t);				break;
		case E_FLOAT:
			printFloat(dynamic_cast<const Float *>(e), t);			break;
		case E_STRING:
			printString(dynamic_cast<const String *>(e), t);		break;
		
		case E_SUM: case E_SUB:
		case E_MUL: case E_POW:
		case E_DIV: case E_MOD:
			printBinary(dynamic_cast<const ExprBinary *>(e), t);	break;
		case E_NEG:
			printUnary(dynamic_cast<const ExprUnary *>(e),  t);	break;
		
		case E_AND: case E_OR:
			printBinary(dynamic_cast<const ExprBinary *>(e), t);	break;
		case E_NOT:
			printUnary(dynamic_cast<const ExprUnary *>(e),  t);	break;	
		
		case E_EQ: case E_NE:
		case E_LT: case E_LE:
		case E_GT: case E_GE:
			printBinary(dynamic_cast<const ExprBinary *>(e), t);	break;
		
		case E_FUNC_CALL:
			printEFuncCall(dynamic_cast<const EFuncCall *>(e), t);	break;
	}
}

void XML::printVar(const Var *e, int t)
{
	tabs(t); cout << "<variable name='" << e->getValue() << "' />" << endl;
}

void XML::printBool(const Bool *e, int t)
{
	tabs(t); cout << (e->getValue() ? "<true />" : "<false />") << endl;
}

void XML::printInt(const Int *e, int t)
{
	tabs(t); cout << "<integer value='" << e->getValue() << "' />" << endl;
}

void XML::printFloat(const Float *e, int t)
{
	tabs(t); cout << "<float value='" << e->getValue() << "' />" << endl;
}

void XML::printString(const String *e, int t)
{
	tabs(t); cout << "<![CDATA[" << e->getValue() << "]]>" << endl;
}

void XML::printBinary(const ExprBinary *e, int t)
{
	const string& name = tagsExprs[e->getID()];
	
	tabs(t); cout << "<" << name << ">" << endl;
	tabs(t); cout << "\t<first>" << endl;
	printExpr(e->getFirst(), t + 2);
	tabs(t); cout << "\t</first>" << endl;
	tabs(t); cout << "\t<second>" << endl;
	printExpr(e->getSecond(), t + 2);
	tabs(t); cout << "\t</second>" << endl;
	tabs(t); cout << "</" << name << ">" << endl;
}

void XML::printUnary(const ExprUnary *e, int t)
{
	const string& name = tagsExprs[e->getID()];
	
	tabs(t); cout << "<" << name << ">" << endl;
	printExpr(e->getValue(), t + 1);
	tabs(t); cout << "</" << name << ">" << endl;
}

void XML::printEFuncCall(const EFuncCall *e, int t)
{
	const string& name   = e->getName();
	const ExprList& args = e->getArgs();
	ExprList::const_iterator i;
	
	if (args.empty())
	{
		tabs(t); cout << "<function-call name='" << name << "' />" << endl;
	}
	else
	{
		tabs(t); cout << "<function-call name='" << name << "'>" << endl;
		for (i = args.begin(); i != args.end(); i++)
		{
			tabs(t); cout << "\t<argument>" << endl;
			printExpr(*i, t + 2);
			tabs(t); cout << "\t</argument>" << endl;
		}
		tabs(t); cout << "</function-call>" << endl;
	}
}

const string XML::typeName(int type)
{
	if (type == T_VOID)		return "void";
	if (type == T_BOOL)		return "boolean";
	if (type == T_INT)		return "integer";
	if (type == T_FLOAT)	return "float";
	if (type == T_STRING)	return "string";
}

void XML::tabs(int num)
{
	for (int i = 0; i < num; i++)
	{
		cout << "\t";
	}
}