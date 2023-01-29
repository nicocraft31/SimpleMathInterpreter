#include <iostream>
#include <fstream>
#include <string>
#include "interpreter.h"

void __error(const char* msg, Token* token)
{
	if (token == nullptr)
	{
		std::cout << msg << std::endl;
	}
	else
	{
		std::cout << msg << token->get_str_from_type() << std::endl;
	}

	exit(1);
}

std::string read_from_file(const char* path)
{
	std::string file;

	std::ifstream in(path);
	std::string line;
	while (getline(in, line)) {
		file += line += "\n";
	}
	in.close();

	std::cout << file;

	file += -1;
	return file;
}

int main(void)
{
	const char* filepath = "E:/Dev/C++/Language/Language/res/test.lang";
	std::string file = read_from_file(filepath);

	Lexer* lexer = new Lexer(file);
	lexer->start_lexing();

	Parser* parser = new Parser(lexer);
	parser->start_parsing();

	Interpreter* interpreter = new Interpreter(parser);
	interpreter->start_interpreting();

	return 0;
}