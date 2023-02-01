#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(Parser* parser) :
	m_parser(parser), m_expression_length(parser->expressions().size())
{
}

void Interpreter::start_interpreting()
{
	std::vector<Expression*>& expressions = m_parser->expressions();

	for (m_expression_index = 0; m_expression_index < m_expression_length; m_expression_index++)
	{
		Expression* expression = expressions[m_expression_index];

		InterpreterOperation* operation = ast_finished_expression(expression);
	}
}

InterpreterOperation* Interpreter::ast_finished_expression(Expression* expression)
{
	switch (expression->expression_type())
	{
	case EXPRESSION_BINARY:
	{
		ast_finished_binary_expression((BinaryExpression*)expression);
		return nullptr;
	}
	}

	return nullptr;
}

BinaryNumberExpression* Interpreter::ast_finished_binary_expression(BinaryExpression* binary)
{
	switch (binary->binary_type())
	{
	case BINARY_NUMBER:
	{
		return (BinaryNumberExpression*) ast_finished_number_expression((NumberExpression*)binary);
	}
	}

	BinaryNumberExpression* binary_left = binary->left;
	BinaryNumberExpression* binary_right = binary->right;

	BinaryNumberExpression* left = ast_finished_binary_expression((BinaryExpression*)binary_left);
	InterpreterOperation* binary_op = ast_finished_binary_op_expression(binary);

	BinaryNumberExpression* next_binary_expression = (BinaryNumberExpression*)binary_right;
	ast_finished_compute_expression(left, next_binary_expression, binary_op);

	BinaryNumberExpression* right = ast_finished_binary_expression((BinaryExpression*)binary_right);
	if (!left || !right)
		return binary;

	return binary;
}

int Interpreter::ast_finished_compute_expression(BinaryNumberExpression* left, BinaryNumberExpression* right, InterpreterOperation* binary_op)
{
	NumberExpression* number_left = (NumberExpression*)left;
	NumberExpression* number_right = nullptr;

	BinaryExpression* binary_right = (BinaryExpression*)right;

	if (right->binary_type() == BINARY_NUMBER)
		number_right = (NumberExpression*)right;
	else
		number_right = (NumberExpression*) binary_right->left;

	int left_int = number_left->number->value.int_value;
	int right_int = number_right->number->value.int_value;

	int result = 0;

	switch (binary_op->type)
	{
	case OPERATION_DIVIDE:
	{
		result = left_int / right_int;
		break;
	}
	case OPERATION_MULTIPLY:
	{
		result = left_int * right_int;
		break;
	}
	case OPERATION_PLUS:
	{
		result = left_int + right_int;
		break;
	}
	case OPERATION_MINUS:
	{
		result = left_int - right_int;
		break;
	}
	}

	std::cout << "result: " << result << std::endl;

	return result;
}

InterpreterOperation* Interpreter::ast_finished_binary_op_expression(BinaryExpression* binary)
{
	switch (binary->type)
	{
	case BINARY_DIVIDE:
		return OPERATION_NEW(OPERATION_DIVIDE);
	case BINARY_MULTIPLY:
		return OPERATION_NEW(OPERATION_MULTIPLY);
	case BINARY_PLUS:
		return OPERATION_NEW(OPERATION_PLUS);
	case BINARY_MINUS:
		return OPERATION_NEW(OPERATION_MINUS);
	}
}

NumberExpression* Interpreter::ast_finished_number_expression(NumberExpression* number)
{
	return number;

	Token* token = number->number;
	if (token->type == TOKEN_INT)
	{
		std::cout << token->value.int_value << std::endl;
	}
	else if (token->type == TOKEN_FLOAT)
	{
		std::cout << token->value.float_value << std::endl;
	}
	else
	{
		std::cout << token->value.double_value << std::endl;
	}
}
