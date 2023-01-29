#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(Parser* parser) :
	m_parser(parser)
{
}

void Interpreter::start_interpreting()
{
	std::vector<Expression*>& expressions = m_parser->expressions();

	for (int i = 0; i < expressions.size(); i++)
	{
		Expression* expression = expressions[i];

		std::cout << i << std::endl;

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
	BinaryNumberExpression* right = ast_finished_binary_expression((BinaryExpression*)binary_right);
	if (!left || !right)
		return binary;
	
	std::cout << "left: " <<((NumberExpression*)left)->number->value.int_value << std::endl;
	if (right->binary_type() == BINARY_NUMBER) std::cout << "right: " << 
		((NumberExpression*)right)->number->value.int_value << std::endl;
	else 
	{
		BinaryExpression* right_left = (BinaryExpression*)((BinaryExpression*)right)->left;

		std::cout << "right: " << ((NumberExpression*)right_left)->number->value.int_value << std::endl;
	}

	

	return binary;


	BinaryExpression* binary_right_right = (BinaryExpression*)right;
	if(left->binary_type() == BINARY_NUMBER && (right->binary_type() == BINARY_NUMBER || binary_right_right->right->binary_type() == BINARY_NUMBER))
	{
		int left_int = ((NumberExpression*)left)->number->value.int_value;
		int right_int = 0;

		if (right->binary_type() == BINARY_NUMBER)
		{
			NumberExpression* number = (NumberExpression*)right;

			std::cout << "right is number" << std::endl;
			right_int = number->number->value.int_value;
		}
		else if(binary_right_right->right->binary_type() == BINARY_NUMBER)
		{
			NumberExpression* number = (NumberExpression*)binary_right_right->right;

			std::cout << "right of right is number" << std::endl;
			right_int = number->number->value.int_value;
		}
		else
		{
			error("Couldn't set right_int at ast_finished_binary_expression() in interpreter.cpp", nullptr);
		}

		std::cout << "left: " << left_int << " right:" << right_int << std::endl;

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
	}

	return binary;
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