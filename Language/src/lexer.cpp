#include "lexer.h"
#include <iostream>

Lexer::Lexer(std::string& file) :
	m_file(file), m_file_length(file.length())
{
}

void Lexer::start_lexing()
{
	std::string word = "";

	for (char c : m_file)
	{
		if (c == '\r') continue;
		char next_c = m_file[m_text_index + 1];

		if (must_check_push_token(c))
		{
			if (c == '\n')
			{
				m_current_col = 1;
				m_current_line++;
			}

			check_push_token_type(word);

			if (has_hit_token_character(c))
					check_push_token_type_char(c);

			word = "";
		}
		else
		{
			m_current_col++;
			word += c;
		}

		if (c == -1)
		{
			// Hit EOF

			TokenValue value;
			value.string_value = std::string("EOF");
			PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_EOF, value));
		}

		m_text_index++;
	}
}

bool Lexer::must_check_push_token(const char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == -1 || has_hit_token_character(c);
}

bool Lexer::has_hit_token_character(char c)
{
	switch (c)
	{
	case TOKEN_PLUS:
		return true;
	case TOKEN_MINUS:
		return true;
	case TOKEN_EQUALS:
		return true;
	case TOKEN_LPAREN:
		return true;
	case TOKEN_RPAREN:
		return true;
	case TOKEN_DIVIDE:
		return true;
	case TOKEN_MULTIPLY:
		return true;
	case TOKEN_SEMICOLON:
		return true;
	}

	return false;
}

void Lexer::check_push_token_type_char(const char c)
{
	switch (c)
	{
	case TOKEN_EQUALS:
	{
		TokenValue value;
		value.char_value = '=';
		value.string_value = "=";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_EQUALS, value));
		return;
	}
	case TOKEN_PLUS:
	{
		TokenValue value;
		value.char_value = '+';
		value.string_value = "+";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_PLUS, value));
		return;
	}
	case TOKEN_MINUS:
	{
		TokenValue value;
		value.char_value = '-';
		value.string_value = "-";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_MINUS, value));
		return;
	}
	case TOKEN_DIVIDE:
	{
		TokenValue value;
		value.char_value = '/';
		value.string_value = "/";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_DIVIDE, value));
		return;
	}
	case TOKEN_MULTIPLY:
	{
		TokenValue value;
		value.char_value = '*';
		value.string_value = "*";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_MULTIPLY, value));
		return;
	}
	case TOKEN_LPAREN:
	{
		TokenValue value;
		value.char_value = '(';
		value.string_value = "(";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_LPAREN, value));
		return;
	}
	case TOKEN_RPAREN:
	{
		TokenValue value;
		value.char_value = ')';
		value.string_value = ")";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_RPAREN, value));
		return;
	}
	case TOKEN_SEMICOLON:
	{
		TokenValue value;
		value.char_value = ';';
		value.string_value = ";";
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_SEMICOLON, value));
		return;
	}
	}
}

void Lexer::check_push_token_type(std::string& word)
{
	if (word == "") 
		return;

	char first = word[0];
	char last = word[word.length() - 1];

	bool is_number = false;
	bool is_float_number = word.find(".") != std::string::npos;
	bool is_double_number = (last == 'd' || last == 'D') && is_float_number;
	
	for (char c : word)
	{
		if (c == 0) 
			return;

		is_number = (c >= '0' && c <= '9');
	}

	if (is_number)
	{
		if (is_double_number)
		{
			TokenValue value;
			value.double_value = std::stod(word);
			value.string_value = word;
			PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_DOUBLE, value));
			return;
		}

		if (is_float_number)
		{
			TokenValue value;
			value.float_value = std::stof(word);
			value.string_value = word;
			PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_FLOAT, value));
			return;
		}

		TokenValue value;
		value.int_value = std::stoi(word);
		value.string_value = word;

		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_INT, value));
		return;
	}

	if (has_hit_token_character(first) && word.length() == 1)
	{
		check_push_token_type_char(first);
	}
	else
	{
		// It's just an ID :)
		TokenValue value;
		value.string_value = word;
		PUSH_TOKEN(TOKEN_NEW(location(), TOKEN_ID, value));
		return;
	}
}

Token::Token(TokenLocation locationz, TokenType typez, TokenValue valuez) :
	location(locationz), type(typez), value(valuez)
{
}