#include "Lex.h"

Lex::Lex() : in(0) {}

Lex::Lex(std::istream *in) : in(in)
{
	line = 1;
	col  = 0;
	nextc();
}

void Lex::setInput(std::istream *in)
{
	this->in = in;
	
	line = 1;
	col  = 0;
	nextc();
}

#define TOKEN(type) Token(type, #type)
#define TOKEN2(type, data) Token(type, data, #type)

Token Lex::next()
{
	if (!in)
	{
		throw LexError("Stream is not set", 0, 0);
	}
	
	skipWhitespaces();
	if (in->eof())
	{
		return TOKEN(T_EOF);
	}
	
	if (c == '=')
	{
		nextc();
		return TOKEN(T_EQ);
	}
	if (c == '+')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_SUM_ASSIGN);
		}
		if (c == '+')
		{
			nextc();
			return TOKEN(T_INC);
		}
		return TOKEN(T_PLUS);
	}
	if (c == '-')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_SUB_ASSIGN);
		}
		if (c == '-')
		{
			nextc();
			return TOKEN(T_DEC);
		}
		return TOKEN(T_MINUS);
	}
	if (c == '*')
	{
		nextc();
		if (c == '*')
		{
			nextc();
			if (c == '=')
			{
				nextc();
				return TOKEN(T_POW_ASSIGN);
			}
			return TOKEN(T_POW);
		}
		if (c == '=')
		{
			nextc();
			return TOKEN(T_MUL_ASSIGN);
		}
		return TOKEN(T_MUL);
	}
	if (c == '/')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_DIV_ASSIGN);
		}
		if (c == '*')
		{
			skipCComment();
			return next();
		}
		if (c == '/')
		{
			skipCppComment();
			return next();
		}
		return TOKEN(T_DIV);
	}
	if (c == '%')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_MOD_ASSIGN);
		}
		return TOKEN(T_MOD);
	}
	if (c == '!')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_NE);
		}
		return TOKEN(T_NOT);
	}
	if (c == '<')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_LE);
		}
		return TOKEN(T_LT);
	}
	if (c == '>')
	{
		nextc();
		if (c == '=')
		{
			nextc();
			return TOKEN(T_GE);
		}
		return TOKEN(T_GT);
	}
	if (c == ';')
	{
		nextc();
		return TOKEN(T_SEMICOLON);
	}
	if (c == ',')
	{
		nextc();
		return TOKEN(T_COMMA);
	}
	if (c == '(')
	{
		nextc();
		return TOKEN(T_LEFT);
	}
	if (c == ')')
	{
		nextc();
		return TOKEN(T_RIGHT);
	}
	if (c == '{')
	{
		nextc();
		return TOKEN(T_LEFT_CURLY);
	}
	if (c == '}')
	{
		nextc();
		return TOKEN(T_RIGHT_CURLY);
	}
	if (c == '\'' || c == '"')
	{
		return TOKEN2(T_STRING, readString(c));
	}
	
	string data = readNumberOrIdentifier();
	if (data == "")
	{
		int old = c;
		nextc();
		throw LexError(string("Unknown token '") + (char) old + "'", line, col);
	}
	
	if (isdigit(data[0]))
	{
		if (isIntNumber(data))
		{
			return TOKEN2(T_INTEGER_CONSTANT, data);
		}
		if (isFloatNumber(data))
		{
			return TOKEN2(T_FLOAT_CONSTANT, data);
		}
		throw LexError("Invalid number '" + data + "'", line, col);
	}
	
	Keywords::const_iterator i = keywords.find(data);
	if (i != keywords.end())
	{
		return TOKEN2(i->second, i->first);
	}
	
	return TOKEN2(T_IDENTIFIER, data);
}

// FIXME eof and other shit
inline const string Lex::readString(char quote)
{
	string data;
	
	nextc();
	for (;;)
	{
		if (in->eof())
		{
			throw LexError("Unexpected EOF in string", line, col);
		}
		if (c == quote)
		{
			nextc();
			break;
		}
		if (c == '\\')
		{
			nextc();
			if (c == quote)
			{
				data += quote;
				nextc();
				continue;
			}
			else if (c == '\\')
			{
				data += '\\';
				nextc();
				continue;
			}
			else if (c == 'n')
			{
				// data += '\n';
				data += "\\n";
				nextc();
				continue;
			}
			else if (c == 't')
			{
				// data += '\t';
				data += "\\t";
				nextc();
				continue;
			}
			// TODO: add other escape characters?
			throw LexError("Unexpected '\\' in string", line, col);
		}
		data += c;
		nextc();
	}
	
	return data;
}

inline const string Lex::readNumberOrIdentifier()
{
	string result;
	
	while (!in->eof() && (isdigit(c) || c == '.' || isalpha(c) || c == '_'))
	{
		result += c;
		nextc();
	}
	
	return result;
}

inline bool Lex::isIntNumber(const string& data)
{
	char *endptr;
	strtol(data.c_str(), &endptr, 10);
	return endptr == data.c_str() + data.length();
}

inline bool Lex::isFloatNumber(const string& data)
{
	char *endptr;
	strtof(data.c_str(), &endptr);
	return endptr == data.c_str() + data.length();
}

inline void Lex::skipWhitespaces()
{
	while (!in->eof() && isspace(c))
	{
		nextc();
	}
}

// skip C style comment: /* multiline comment */
inline void Lex::skipCComment()
{
	while (!in->eof())
	{
		nextc();
		if (c == '*')
		{
			nextc();
			if (c == '/')
			{
				nextc();
				break;
			}
		}
	}
}

// skip C++ style comment: // single line comment
inline void Lex::skipCppComment()
{
	while (!in->eof() && c != '\n')
	{
		nextc();
	}
}

// get next character from input stream
inline void Lex::nextc()
{
	c = in->get();
	
	if (c == '\n')
	{
		line++;
		col = 0;
	}
	else
	{
		col++;
	}
}