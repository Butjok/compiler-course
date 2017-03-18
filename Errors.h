#pragma once

#include <string>

class Error
{
	std::string w;
	int line, col;

public:
	Error(const std::string& w, int line, int col)
		: w(w), line(line), col(col) {}
	
	const std::string& what() const { return w; }
	
	int getLine() const { return line; }
	int getCol()  const { return col; }
};

class LexError : public Error
{
public:
	LexError(const std::string& w, int line, int col)
		: Error(w, line, col) {}
};

class ParserError : public Error
{
public:
	ParserError(const std::string& w, int line, int col)
		: Error(w, line, col) {}
};

class AnalyzerError : public Error
{
	int getLine() const { return 0; }
	int getCol()  const { return 0; }
	
public:
	AnalyzerError(const std::string& w)
		: Error(w, 0, 0) {}
};