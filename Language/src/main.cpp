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
	const char* path_c = "E:/Dev/C++/Language/Language/res/test.lang";
	std::string str = read_from_file(path_c);
	std::string path(path_c);

	Lexer* lexer = new Lexer(str);
	lexer->start_lexing();

	Parser* parser = new Parser(lexer);
	parser->start_parsing();

	Interpreter* interpreter = new Interpreter(parser);

	return 0;
}