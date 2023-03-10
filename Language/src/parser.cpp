#include "parser.h"
#include <iostream>

Parser::Parser(Lexer* lexer) :
	m_lexer(lexer), m_current_token(lexer->tokens()[0]), m_expressions(std::vector<Expression*>())
{
}

void Parser::start_parsing()
{
	m_current_token = m_lexer->tokens()[0];

	for (int i = 0; i < m_lexer->token_length(); i++)
	{
		Token* token = nullptr;
		token = peek_next();
		Expression* expression = (Expression*)parse_expression(token);
		if(expression != nullptr)
			EXPRESSION_PUSH(expression);

		if (token->type == TOKEN_EOF)
			break;
	}
}

Expression* Parser::parse_expression(Token* token)
{
	if (is_token_number(token))
	{
		return parse_binary(token);
	}

	if(token->type != TOKEN_EOF)
		error("Unexpected token: ", token);

	return nullptr;
}

BinaryExpression* Parser::parse_binary(Token* token)
{
	NumberExpression* left = parse_number(token);
	VERIFY_NULL(left, "Expected number (int, float, or double), but got: ", m_current_token);
	BinaryExpressionType binary_op = parse_binary_op();
	VERIFY_NULL(binary_op, "Expected valid binary_op, but got: ", m_current_token);
	BinaryNumberExpression* right = parse_binary_number(peek_next());
	VERIFY_NULL(right, "Expected number or binary expression, but got: ", m_current_token);

	BinaryExpression* expression = EXPRESSION_NEW(BinaryExpression(binary_op, (BinaryNumberExpression*)left, (BinaryNumberExpression*)right));
	expression->type = binary_op;
	std::cout << "success!" << std::endl;

	return expression;
}

BinaryNumberExpression* Parser::parse_binary_number(Token* token)
{
	Token* next_token = peek_not_consume();

	if (is_token_number(token))
	{
		if (is_token_binary(next_token))
		{
			return (BinaryNumberExpression*)parse_binary(token);
		}
		else if (is_token_number(next_token))
		{
			error("Expected binary operator, but got: ", next_token);
		}

		return (BinaryNumberExpression*) parse_number(token);
	}

	return nullptr;
}

BinaryExpressionType Parser::parse_binary_op()
{
	Token* token = peek_next();

	if (is_token_binary(token))
	{
		switch (token->type)
		{
		case TOKEN_DIVIDE:
			return BINARY_DIVIDE;
		case TOKEN_PLUS:
			return BINARY_PLUS;
		case TOKEN_MINUS:
			return BINARY_MINUS;
		case TOKEN_MULTIPLY:
			return BINARY_MULTIPLY;
		}
	}

	error("Expected binary operator symbol, but got: ", token);
	return BINARY_NONE;
}

NumberExpression* Parser::parse_number(Token* token)
{
	if (is_token_number(token))
	{
		return EXPRESSION_NEW(NumberExpression(token));
	}

	error("Expected number, but got: ", token);
}

BinaryExpression::BinaryExpression(BinaryExpressionType typez, BinaryNumberExpression* leftz, BinaryNumberExpression* rightz) :
	left(leftz), right(rightz)
{
}

NumberExpression::NumberExpression(Token* token) : 
	number(token)
{
}