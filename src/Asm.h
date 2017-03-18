#pragma once

#include "Analyzer.h"

using std::ostream;
using std::endl;

typedef list<const VarDef *> VarDefList;

class AsmVar
{
	int type;
	int offset;
	
public:
	AsmVar() : type(0), offset(0) {}
	AsmVar(int type, int offset) : type(type), offset(offset) {}
	
	int getType() const		{ return type; }
	int getOffset() const	{ return offset; }
};

typedef map<string, AsmVar> AsmVarMap;

class Asm
{
	ostream& out;
	int offset;
	Analyzer analyzer;
	list<string> strings;
	int jmpCount;
	
public:
	Asm(ostream& out) : out(out), offset(0) {}
	
	void asmProgram(const Program& program)
	{
		const FuncMap& funcs = program.getFuncs();
		
		FuncMap::const_iterator i;
		for (i = funcs.begin(); i!= funcs.end(); i++)
		{
			if (i->second.isExtern())
			{
				continue;
			}
			asmFunction(i->second, funcs);
		}
		
		out << ".TRUE:" << endl;
		out << "	.string \"true\"" << endl;
		out << ".FALSE:" << endl;
		out << "	.string \"false\"" << endl;
		out << endl;
		
		list<string>::const_iterator j;
		int index;
		for (j = strings.begin(), index = 0; j != strings.end(); ++j, ++index)
		{
			out << ".LC" << index << ":" << endl;
			out << "	.string \"" << *j << "\"" << endl;
		}
	}
	
	void asmFunction(const Func& func, const FuncMap& funcs)
	{
		out << ".globl " << func.getName() << endl;
		out << "" << func.getName() << ":" << endl;
		out << "	pushl	%ebp" << endl;
		out << "	movl	%esp, %ebp" << endl;
		
		AsmVarMap vars;
		
		const VarMap& args = func.getArgs();
		VarMap::const_iterator j;
		int offset = 4;
		for (j = args.begin(); j != args.end(); j++)
		{
			offset += 4;
			vars[j->first] = AsmVar(j->second, offset);
		}
		
		VarDefList varDefs;
		getVarDefs(func.getBody(), varDefs, vars, func, funcs);
		
		if (varDefs.size() > 0)
		{
			int offset = 0;
			VarDefList::const_iterator i;
			for (i = varDefs.begin(); i != varDefs.end(); i++)
			{
				const VarDef *varDef = *i;
				offset -= 4;
				vars[varDef->getName()] = AsmVar(varDef->getType(), offset);
			}
			
			out << "	subl	$" << (varDefs.size() * 4) << ", %esp" << endl;
		}
		
		asmBody(func.getBody(), vars, func, funcs);
		
		out << ".RETURN_" << func.getName() << ":" << endl;
		out << "	addl	$" << (varDefs.size() * 4) << ", %esp" << endl;
		out << "	leave" << endl;
		out << "	ret" << endl;
		out << endl;
	}
	
	void asmExpr(const Expr *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		switch (e->getID())
		{
			case E_VAR:
				asmVar(dynamic_cast<const Var *>(e), vars, func, funcs);		break;
			
			case E_BOOL:
				asmBool(dynamic_cast<const Bool *>(e), vars, func, funcs);		break;
			case E_INT:
				asmInt(dynamic_cast<const Int *>(e), vars, func, funcs);		break;
			case E_FLOAT:
				asmFloat(dynamic_cast<const Float *>(e), vars, func, funcs);	break;
			case E_STRING:
				asmString(dynamic_cast<const String *>(e), vars, func, funcs);	break;
				
			case E_SUM:
				asmSum(dynamic_cast<const Sum *>(e), vars, func, funcs);		break;
			case E_SUB:
				asmSub(dynamic_cast<const Sub *>(e), vars, func, funcs);		break;
			case E_MUL:
				asmMul(dynamic_cast<const Mul *>(e), vars, func, funcs);		break;
			case E_POW:
				// asmPow(dynamic_cast<const Pow *>(e), vars, func, funcs); break;
			case E_DIV:
				asmDiv(dynamic_cast<const Div *>(e), vars, func, funcs);		break;
			case E_MOD:
				asmMod(dynamic_cast<const Mod *>(e), vars, func, funcs);		break;
			case E_NEG:
				asmNeg(dynamic_cast<const Neg *>(e), vars, func, funcs);		break;
				
			case E_AND:
				asmAnd(dynamic_cast<const And *>(e), vars, func, funcs);		break;
			case E_OR:
				asmOr(dynamic_cast<const Or *>(e), vars, func, funcs);			break;
			case E_NOT:
				asmNot(dynamic_cast<const Not *>(e), vars, func, funcs);		break;
			
			case E_EQ:
				asmEq(dynamic_cast<const Eq *>(e), vars, func, funcs);			break;
			case E_NE:
				asmNe(dynamic_cast<const Ne *>(e), vars, func, funcs);			break;
			case E_LT:
				asmLt(dynamic_cast<const Lt *>(e), vars, func, funcs);			break;
			case E_LE:
				asmLe(dynamic_cast<const Le *>(e), vars, func, funcs);			break;
			case E_GT:
				asmGt(dynamic_cast<const Gt *>(e), vars, func, funcs);			break;
			case E_GE:
				asmGe(dynamic_cast<const Ge *>(e), vars, func, funcs);			break;
			
			case E_FUNC_CALL:
				asmEFuncCall(dynamic_cast<const EFuncCall *>(e), vars, func, funcs);	break;
		}
	}
	
	void asmVar(const Var *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		AsmVarMap::const_iterator i = vars.find(e->getValue());
		out << "	pushl	" << i->second.getOffset() << "(%ebp)" << endl;
	}
	
	void asmBool(const Bool *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		out << "	pushl	$" << (e->getValue() ? "1" : "0") << endl;
	}
	
	void asmInt(const Int *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		out << "	pushl	$" << e->getValue() << endl;
	}
	
	void asmFloat(const Float *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		// TODO: byte order, hex print are both shitty
		float value = e->getValue();
		char hex[9] = {0};
		unsigned char *p = (unsigned char *) &value;
		sprintf(hex, "%02x%02x%02x%02x", p[3], p[2], p[1], p[0]);
		out << "	pushl	$0x" << hex << endl;
	}
	
	void asmString(const String *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		out << "	pushl	$.LC" << strings.size() << endl;
		strings.push_back(e->getValue());
	}
	
	void convert(int from, int to)
	{
		if (from == to)
		{
			return;
		}
		if (from == T_INT && to == T_FLOAT)
		{
			out << "	fildl	(%esp)" << endl;
			out << "	fstps	(%esp)" << endl;
		}
		if (from == T_FLOAT && to == T_INT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	fistpl	(%esp)" << endl;
		}
	}
	
	void asmSum(const Sum *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	addl	%ebx, %eax" << endl;
			out << "	pushl	%eax" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	faddp" << endl;
			out << "	addl	$4, %esp" << endl;
			out << "	fstps	(%esp)" << endl;
		}
	}
	
	void asmSub(const Sub *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	subl	%ebx, %eax" << endl;
			out << "	pushl	%eax" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fsubp" << endl;
			out << "	addl	$4, %esp" << endl;
			out << "	fstps	(%esp)" << endl;
		}
	}
	
	void asmMul(const Mul *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	imull	%ebx" << endl;
			out << "	pushl	%eax" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fmulp" << endl;
			out << "	addl	$4, %esp" << endl;
			out << "	fstps	(%esp)" << endl;
		}
	}
	
	void asmDiv(const Div *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	xorl	%edx, %edx" << endl;
			out << "	idivl	%ebx" << endl;
			out << "	pushl	%eax" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fdivp" << endl;
			out << "	addl	$4, %esp" << endl;
			out << "	fstps	(%esp)" << endl;
		}
	}
	
	void asmMod(const Mod *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		asmExpr(e->getFirst(),  vars, func, funcs);
		asmExpr(e->getSecond(), vars, func, funcs);
		out << "	popl	%ebx" << endl;
		out << "	popl	%eax" << endl;
		out << "	xorl	%edx, %edx" << endl;
		out << "	idivl	%ebx" << endl;
		out << "	pushl	%edx" << endl;
	}
	
	void asmNeg(const Neg *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const Expr *value = e->getValue();
		int type = getType(value, vars, func, funcs);
		
		asmExpr(value, vars, func, funcs);
		
		if (type == T_INT)
		{
			out << "	popl	%eax" << endl;
			out << "	negl	%eax" << endl;
			out << "	pushl	%eax" << endl;
		}
		else if (type == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	fchs" << endl;
			out << "	fstps	(%esp)" << endl;
		}
	}
	
	void asmAnd(const And *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		asmExpr(e->getFirst(), vars, func, funcs);
		asmExpr(e->getSecond(), vars, func, funcs);
		out << "	popl	%ebx" << endl;
		out << "	popl	%eax" << endl;
		out << "	andb	%bl, %al" << endl;
		out << "	pushl	%eax" << endl;
	}
	
	void asmOr(const Or *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		asmExpr(e->getFirst(), vars, func, funcs);
		asmExpr(e->getSecond(), vars, func, funcs);
		out << "	popl	%ebx" << endl;
		out << "	popl	%eax" << endl;
		out << "	orb	%bl, %al" << endl;
		out << "	pushl	%eax" << endl;
	}
	
	void asmNot(const Not *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		asmExpr(e->getValue(), vars, func, funcs);
		out << "	popl	%eax" << endl;
		out << "	xorb	$1, %al" << endl;
		out << "	pushl	%eax" << endl;
	}
	
	void asmEq(const Eq *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	seteb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	seteb	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmNe(const Ne *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	setneb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	setneb	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmLt(const Lt *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	setlb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	setbb	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmLe(const Le *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	setleb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	setbeb	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmGt(const Gt *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	setgb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	setab	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmGe(const Ge *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int type1 = getType(e->getFirst(),  vars, func, funcs);
		int type2 = getType(e->getSecond(), vars, func, funcs);
		int resultType = (type1 == T_INT && type2 == T_INT) ? T_INT : T_FLOAT;
		
		asmExpr(e->getFirst(),  vars, func, funcs);
		convert(type1, resultType);
		asmExpr(e->getSecond(), vars, func, funcs);
		convert(type2, resultType);
		
		if (resultType == T_INT)
		{
			out << "	popl	%ebx" << endl;
			out << "	popl	%eax" << endl;
			out << "	cmpl	%ebx, %eax" << endl;
			out << "	setgeb	%cl" << endl;
			out << "	pushl	%ecx" << endl;
		}
		else if (resultType == T_FLOAT)
		{
			out << "	flds	(%esp)" << endl;
			out << "	flds	4(%esp)" << endl;
			out << "	fcompp" << endl;
			out << "	fstsw	%ax" << endl;
			out << "	sahf" << endl;
			out << "	setaeb	%cl" << endl;
			out << "	addl	$8, %esp" << endl;
			out << "	pushl	%ecx" << endl;
		}
	}
	
	void asmEFuncCall(const EFuncCall *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const ExprList& exprs = e->getArgs();
		const VarMap& args = funcs.find(e->getName())->second.getArgs();
		
		// arguments in reversed order
		if (args.size() > 0)
		{
			ExprList::const_reverse_iterator i;
			VarMap::const_reverse_iterator j;
			for (i =  exprs.rbegin(), j =  args.rbegin();
			     i != exprs.rend(),   j != args.rend();
			     i++, j++)
			{
				asmExpr(*i, vars, func, funcs);
				int givenType    = getType(*i, vars, func, funcs);
				int expectedType = j->second;
				convert(givenType, expectedType);
			}
		}
		
		// call function
		out << "	call	" << e->getName() << endl;
		
		// clear stack
		if (args.size() > 0)
		{
			out << "	addl	$" << (args.size() * 4) << ", %esp" << endl;
		}
		
		// push function result from %eax into stack
		out << "	pushl	%eax" << endl;
	}
	
	void getVarDefs(const InstrList& instrs, VarDefList& varDefs, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		InstrList::const_iterator i;
		for (i = instrs.begin(); i != instrs.end(); i++)
		{
			if ((*i)->getID() == I_VAR_DEF)
			{
				varDefs.push_back(dynamic_cast<const VarDef *>(*i));
			}
			if ((*i)->getID() == I_IF)
			{
				getVarDefs(dynamic_cast<const If *>(*i)->getTrueBody(),  varDefs, vars, func, funcs);
				getVarDefs(dynamic_cast<const If *>(*i)->getFalseBody(), varDefs, vars, func, funcs);
			}
			if ((*i)->getID() == I_WHILE)
			{
				getVarDefs(dynamic_cast<const While *>(*i)->getBody(), varDefs, vars, func, funcs);
			}
			if ((*i)->getID() == I_FOR)
			{
				getVarDefs(dynamic_cast<const For *>(*i)->getInit(), varDefs, vars, func, funcs);
				getVarDefs(dynamic_cast<const For *>(*i)->getStep(), varDefs, vars, func, funcs);
				getVarDefs(dynamic_cast<const For *>(*i)->getBody(), varDefs, vars, func, funcs);
			}
		}
	}
	
	void asmBody(const InstrList& body, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		InstrList::const_iterator i;
		for (i = body.begin(); i != body.end(); i++)
		{
			switch ((*i)->getID())
			{
				case I_VAR_DEF:
					asmVarDef(dynamic_cast<const VarDef *>(*i), vars, func, funcs);			break;
				
				case I_VAR_ASSIGN:
					asmVarAssign(dynamic_cast<const VarAssign *>(*i), vars, func, funcs);		break;
				case I_VAR_SUM_ASSIGN:
					// asmVarSumAssign(dynamic_cast<const VarSumAssign *>(*i), vars, func, funcs);	break;
				case I_VAR_SUB_ASSIGN:
					// asmVarSubAssign(dynamic_cast<const VarSubAssign *>(*i), vars, func, funcs);	break;
				case I_VAR_MUL_ASSIGN:
					// asmVarMulAssign(dynamic_cast<const VarMulAssign *>(*i), vars, func, funcs);	break;
				case I_VAR_POW_ASSIGN:
					// asmVarPowAssign(dynamic_cast<const VarPowAssign *>(*i), vars, func, funcs);	break;
				case I_VAR_DIV_ASSIGN:
					// asmVarDivAssign(dynamic_cast<const VarDivAssign *>(*i), vars, func, funcs);	break;
				case I_VAR_MOD_ASSIGN:
					// asmVarModAssign(dynamic_cast<const VarModAssign *>(*i), vars, func, funcs);	break;
					
				case I_VAR_INC:
					asmVarInc(dynamic_cast<const VarInc *>(*i), vars, func, funcs);			break;
				case I_VAR_DEC:
					asmVarDec(dynamic_cast<const VarDec *>(*i), vars, func, funcs);			break;
					
				case I_RETURN:
					asmReturn(dynamic_cast<const Return *>(*i), vars, func, funcs);			break;
					
				case I_FUNC_CALL:
					asmIFuncCall(dynamic_cast<const IFuncCall *>(*i), vars, func, funcs);	break;
					
				case I_IF:
					asmIf(dynamic_cast<const If *>(*i), vars, func, funcs);					break;
					
				case I_WHILE:
					asmWhile(dynamic_cast<const While *>(*i), vars, func, funcs);			break;
				case I_FOR:
					asmFor(dynamic_cast<const For *>(*i), vars, func, funcs);				break;
					
				case I_PRINT:
					asmPrint(dynamic_cast<const Print *>(*i), vars, func, funcs);			break;
			}
		}
	}
	
	void asmVarDef(const VarDef *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const AsmVar& var = vars[i->getName()];
		const Expr *value = i->getValue();
		
		if (!value)
		{
			return;
		}
		
		int from = getType(value, vars, func, funcs);
		int to   = var.getType();
		
		asmExpr(value, vars, func, funcs);
		convert(from, to);
		
		out << "	popl	" << var.getOffset() << "(%ebp)" << endl;
	}
	
	void asmVarAssign(const VarAssign *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const AsmVar& var = vars[i->getName()];
		const Expr *value = i->getValue();
		
		int from = getType(value, vars, func, funcs);
		int to   = var.getType();
		
		asmExpr(value, vars, func, funcs);
		convert(from, to);
		
		out << "	popl	" << var.getOffset() << "(%ebp)" << endl;
	}
	
	void asmVarInc(const VarInc *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		out << "	incl	" << vars[i->getName()].getOffset() << "(%ebp)" << endl;
	}
	
	void asmVarDec(const VarDec *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		out << "	decl	" << vars[i->getName()].getOffset() << "(%ebp)" << endl;
	}
	
	void asmReturn(const Return *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		if (i->getValue())
		{
			const Expr *value = i->getValue();
			
			int from = getType(value, vars, func, funcs);
			int to   = func.getType();
			
			asmExpr(value, vars, func, funcs);
			convert(from, to);
			
			out << "	popl	%eax" << endl;
		}
		out << "	jmp	.RETURN_" << func.getName() << endl;
	}
	
	void asmIFuncCall(const IFuncCall *e, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		const ExprList& exprs = e->getArgs();
		const VarMap& args = funcs.find(e->getName())->second.getArgs();
		
		// arguments in reversed order
		if (args.size() > 0)
		{
			ExprList::const_reverse_iterator i;
			VarMap::const_reverse_iterator j;
			for (i =  exprs.rbegin(), j =  args.rbegin();
			     i != exprs.rend(),   j != args.rend();
			     i++, j++)
			{
				asmExpr(*i, vars, func, funcs);
				int givenType    = getType(*i, vars, func, funcs);
				int expectedType = j->second;
				convert(givenType, expectedType);
			}
		}
		
		// call function
		out << "	call	" << e->getName() << endl;
		
		// clear arguments from stack
		if (args.size() > 0)
		{
			out << "	addl	$" << (args.size() * 4) << ", %esp" << endl;
		}
	}
	
	void asmIf(const If *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int index = jmpCount++;
		
		asmExpr(i->getCond(), vars, func, funcs);
		out << "	popl	%eax" << endl;
		out << "	decb	%al" << endl;
		out << "	jne	.IF_FALSE_" << index << endl;
		
		// true body
		asmBody(i->getTrueBody(), vars, func, funcs);
		out << "	jmp	.IF_END_" << index << endl;
		
		// false body
		out << ".IF_FALSE_" << index << ":" << endl;
		asmBody(i->getFalseBody(), vars, func, funcs);
		
		out << ".IF_END_" << index << ":" << endl;
	}
	
	void asmWhile(const While *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int index = jmpCount++;
		
		out << ".WHILE_" << index << ":" << endl;

		asmExpr(i->getCond(), vars, func, funcs);
		out << "	popl	%eax" << endl;
		out << "	decb	%al" << endl;
		out << "	jne	.WHILE_END_" << index << endl;
		
		asmBody(i->getBody(), vars, func, funcs);
		out << "	jmp	.WHILE_" << index << endl;
		
		out << ".WHILE_END_" << index << ":" << endl;
	}
	
	void asmFor(const For *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		int index = jmpCount++;
		
		// initialization
		asmBody(i->getInit(), vars, func, funcs);
		
		out << ".FOR_" << index << ":" << endl;
		
		asmExpr(i->getCond(), vars, func, funcs);
		out << "	popl	%eax" << endl;
		out << "	decb	%al" << endl;
		out << "	jne	.FOR_END_" << index << endl;
		
		asmBody(i->getBody(), vars, func, funcs);
		asmBody(i->getStep(), vars, func, funcs);
		out << "	jmp	.FOR_" << index << endl;
		
		out << ".FOR_END_" << index << ":" << endl;
	}
	
	void asmPrint(const Print *i, AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		string format = "";
		
		const ExprList& exprs = i->getArgs();
		ExprList::const_reverse_iterator j;
		for (j = exprs.rbegin(); j != exprs.rend(); ++j)
		{
			int type = getType(*j, vars, func, funcs);
			
			if (type == T_BOOL)
			{
				format = "%s" + format;
			}
			if (type == T_INT)
			{
				format = "%d" + format;
			}
			if (type == T_FLOAT)
			{
				format = "%f" + format;
			}
			if (type == T_STRING)
			{
				format = "%s" + format;
			}
			
			asmExpr(*j, vars, func, funcs);
			
			// place "true" or "false" instead number
			if (type == T_BOOL)
			{
				out << "	movl	$.TRUE, %ecx" << endl;
				out << "	movl	$.FALSE, %edx" << endl;
				out << "	popl	%eax" << endl;
				out << "	decb	%al" << endl;
				out << "	cmovel	%ecx, %ebx" << endl;
				out << "	cmovnel	%edx, %ebx" << endl;
				out << "	pushl	%ebx" << endl;
			}
			
			// float point -> double for printf
			if (type == T_FLOAT)
			{
				out << "	flds	(%esp)" << endl;
				out << "	subl	$4, %esp" << endl;
				out << "	fstpl	(%esp)" << endl;
			}
		}
		
		out << "	pushl	$.LC" << strings.size() << endl;
		strings.push_back(format);
		
		out << "	call	printf" << endl;
		
		out << "	addl	$" << (exprs.size() * 4 + 4) << ", %esp" << endl;
	}
	
	int getType(const Expr *e, const AsmVarMap& vars, const Func& func, const FuncMap& funcs)
	{
		VarMap vvars;
		
		AsmVarMap::const_iterator i;
		for (i = vars.begin(); i != vars.end(); ++i)
		{
			vvars[i->first] = i->second.getType();
		}
		
		return analyzer.typeExpr(e, vvars, func, funcs);
	}
};