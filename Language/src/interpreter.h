#pragma once
#include "parser.h"

enum InterpreterOperationType
{
	OPERATION_PLUS = 0,
	OPERATION_MINUS = 1,
	OPERATION_MULTIPLY = 2,
	OPERATION_DIVIDE = 3,
};

struct InterpreterOperation
{
	InterpreterOperationType type;

	InterpreterOperation(InterpreterOperationType typez) :
		type(typez)
	{
	}
};

#define OPERATION_NEW(type) new InterpreterOperation(type)
class Interpreter
{
public:
	Interpreter(Parser* parser);

	void start_interpreting();

	InterpreterOperation* ast_finished_expression(Expression* expression);
	NumberExpression* ast_finished_number_expression(NumberExpression* number);
	InterpreterOperation* ast_finished_binary_op_expression(BinaryExpression* binary);
	BinaryNumberExpression* ast_finished_binary_expression(BinaryExpression* binary);
	BinaryNumberExpression* ast_finished_binary_expression(BinaryNumberExpression* binary_number);
private:
	Parser* m_parser;
private:
};