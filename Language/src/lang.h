#pragma once
#include <string>

struct Token;
struct Language
{
	std::string file;
	std::string filepath;

	Language(std::string& filez, std::string& filepathz) : file(filez), filepath(filepathz) {}
};

void __error(const char* msg, Token* token);

#define error(x, token) __error(x, token)