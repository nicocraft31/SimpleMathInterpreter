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

	for (int i = 0; i < m_expressions.size(); i++)
	{
		Expression* expression = m_expressions[i];
	
		parse_finished_expression(expression);
	}
}

void Parser::parse_finished_expression(Expression* expression)
{
	switch (expression->expression_type())
	{
	case EXPRESSION_BINARY:
	{
		parse_finished_binary_expression((BinaryExpression*)expression);
		return;
	}
	}
}

void Parser::parse_finished_binary_expression(BinaryExpression* binary)
{
	switch (binary->binary_type())
	{
	case BINARY_NUMBER:
	{
		parse_finished_number_expression((NumberExpression*)binary);
		return;
	}
	}

	BinaryNumberExpression* left = binary->left;
	BinaryNumberExpression* right = binary->right;

	parse_finished_binary_expression((BinaryExpression*)left);
	parse_finished_binary_op_expression(binary);
	parse_finished_binary_expression((BinaryExpression*)right);
}

void Parser::parse_finished_binary_op_expression(BinaryExpression* binary)
{
	std::cout << "BinaryOp: " << binary->get_str_from_type() << std::endl;
}

void Parser::parse_finished_number_expression(NumberExpression* number)
{
	Token* token = number->number;
	std::cout << "Number: " << token->get_str_from_type() << " ";
	if (token->type == TOKEN_INT)
	{
		std::cout << token->value.int_value << std::endl;
	}
	else if(token->type == TOKEN_FLOAT)
	{
		std::cout << token->value.float_value << std::endl;
	}
	else
	{
		std::cout << token->value.double_value << std::endl;
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
	VERIFY(left, "Expected number (int, float, or double), but got: ", m_current_token);
	BinaryExpressionType binary_op = parse_binary_op();
	VERIFY(binary_op, "Expected valid binary_op, but got: ", m_current_token);
	BinaryNumberExpression* right = parse_binary_number(peek_next());
	VERIFY(right, "Expected number or binary expression, but got: ", m_current_token);

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