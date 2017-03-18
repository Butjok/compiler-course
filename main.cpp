#include <iostream>
#include <fstream>

#include "Parser.h"
#include "XML.h"
#include "Analyzer.h"
#include "Asm.h"

int main(int argc, char **argv)
{
	try
	{
		std::fstream in(argv[1]);
		Lex lex(&in);
		Parser parser(&lex);
		Analyzer analyzer;
		XML xml;
		Asm assm(cout);
		
		assm.asmProgram(analyzer.analyzeProgram(parser.parse()));
	}
	catch (const LexError& e)
	{
		std::cerr << "Lexer error at " << e.getLine() << ":" << e.getCol() << ": " << e.what() << std::endl;
		return 1;
	}
	catch (const ParserError& e)
	{
		std::cerr << "Parser error at " << e.getLine() << ":" << e.getCol() << ": " << e.what() << std::endl;
		return 1;
	}
	catch (const AnalyzerError& e)
	{
		std::cerr << "Analyzer error: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
