#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "lang.h"

enum TokenType
{
	TOKEN_EOF = -6,
	TOKEN_INT = -5,
	TOKEN_FLOAT = -4,
	TOKEN_DOUBLE = -3,
	TOKEN_STRING = -2,
	TOKEN_PLUS = '+',
	TOKEN_EQUALS = '=',
	TOKEN_MINUS = '-',
	TOKEN_LPAREN = '(',
	TOKEN_RPAREN = ')',
	TOKEN_DIVIDE = '/',
	TOKEN_MULTIPLY = '*',
	TOKEN_SEMICOLON = ';',
	TOKEN_ID = -1
};

struct TokenLocation
{
	uint32_t line = 0, col = 0, index = 0;
};

struct TokenValue
{
	int int_value;
	float float_value;
	double double_value;
	std::string string_value;
	char char_value;
	const char* keyword_value;
};

#define TOKEN_NEW(loc, types, values) new Token(loc, types, values)
struct Token
{
public:
	TokenLocation location;
	TokenType type;
	TokenValue value;

	Token(TokenLocation locationz, TokenType typez, TokenValue valuez);

	std::string get_str_from_type()
	{
		switch (type)
		{
		case TOKEN_EOF:
			return "TOKEN_EOF"; 
		case TOKEN_INT:			   
			return "TOKEN_INT"; 
		case TOKEN_PLUS:		   
			return "TOKEN_PLUS"; 
		case TOKEN_MINUS:		   
			return "TOKEN_MINUS"; 
		case TOKEN_EQUALS:		   
			return "TOKEN_EQUALS"; 
		case TOKEN_ID:			   
			return "TOKEN_ID"; 
		case TOKEN_LPAREN:		   
			return "TOKEN_LPAREN"; 
		case TOKEN_RPAREN:		   
			return "TOKEN_RPAREN"; 
		case TOKEN_DIVIDE:		   
			return "TOKEN_DIVIDE"; 
		case TOKEN_FLOAT:		   
			return "TOKEN_FLOAT"; 
		case TOKEN_DOUBLE:		   
			return "TOKEN_DOUBLE";
		case TOKEN_STRING:
			return "TOKEN_STRING";
		case TOKEN_MULTIPLY:
			return "TOKEN_MULTIPLY";
		case TOKEN_SEMICOLON:
			return "TOKEN_SEMICOLON";
		}

		return "Couldn't identify token type";
	}
};

#define PUSH_TOKEN(token) m_token_length++; m_tokens.push_back(token)
struct Lexer
{
	Lexer(std::string& file);

	void start_lexing();
	
	std::string* file() { return &m_file; };

	uint32_t token_index() { return m_token_index; }
	uint32_t token_length() { return m_token_length; };
	Token* peek_no_consume() { if (m_token_index >= m_token_length) return m_tokens[m_token_index-1]; return m_tokens[m_token_index]; }
	Token* peek_next() { if (m_token_index >= m_token_length) return m_tokens[m_token_index - 1]; return m_tokens[m_token_index++]; }
	Token* peek_current() { return m_tokens[m_token_index]; }
	TokenLocation location() { return { m_current_line, m_current_col, m_text_index}; };
	std::vector<Token*>& tokens() { return m_tokens; }
private:
	uint32_t m_current_line = 1;
	uint32_t m_current_col = 1;
	uint32_t m_text_index = 0;

	uint32_t m_token_index = 0;
	uint32_t m_token_length = 0;

	std::vector<Token*> m_tokens;

	std::string m_file;
	uint64_t m_file_length = 0;
private:
	void check_push_token_type(std::string& word);
	void check_push_token_type_char(char c);
	bool must_check_push_token(char c);
	bool has_hit_token_character(char c);
};