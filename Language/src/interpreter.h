#pragma once
#include "parser.h"

class Interpreter
{
public:
	Interpreter(Parser* parser);

	void start_interpreting();
private:
	Parser* m_parser;
private:
};