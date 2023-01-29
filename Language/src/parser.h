#pragma once
#include "lexer.h"

enum ExpressionType
{
	EXPRESSION_NONE = 0,
	EXPRESSION_NUMBER = 1,
	EXPRESSION_BINARY = 2,
};
class Expression
{
public:
	virtual ExpressionType expression_type() = 0;
};

enum BinaryExpressionType
{
	BINARY_NONE = 0,
	BINARY_PLUS = 1,
	BINARY_MINUS = 2,
	BINARY_DIVIDE = 3,
	BINARY_MULTIPLY = 4,
	BINARY_NUMBER = 5
};
struct BinaryNumberExpression : public Expression
{
	virtual BinaryExpressionType binary_type() { return BINARY_DIVIDE; };
};
class NumberExpression : public BinaryNumberExpression
{
public:
	Token* number = nullptr;

	virtual BinaryExpressionType binary_type() override { return BINARY_NUMBER; }
	virtual ExpressionType expression_type() override { return EXPRESSION_NUMBER; }

	NumberExpression(Token* token);
};
class BinaryExpression : public BinaryNumberExpression
{
public:
	BinaryNumberExpression* left = nullptr;
	BinaryNumberExpression* right = nullptr;
	BinaryExpressionType type = BINARY_NONE;

	virtual BinaryExpressionType binary_type() override { return type; }
	virtual ExpressionType expression_type() override { return EXPRESSION_BINARY; }

	BinaryExpression(BinaryExpressionType type, BinaryNumberExpression* left, BinaryNumberExpression* right);

	std::string get_str_from_type()
	{
		switch (type)
		{
		case BINARY_DIVIDE:
			return "BINARY_DIVIDE";
		case BINARY_MINUS:
			return "BINARY_MINUS";
		case BINARY_PLUS:
			return "BINARY_PLUS";
		case BINARY_MULTIPLY:
			return "BINARY_MULTIPLY";
		case BINARY_NUMBER:
			return "BINARY_NUMBER";
		}

		return "";
	}
};


#define EXPRESSION_NEW(x) new x
#define VERIFY(x, msg, token) if(x == 0) {error(msg, token);}
#define EXPRESSION_PUSH(x, type) type* expression= x; m_expressions.push_back(expression)
#define EXPRESSION_PUSH(x) m_expressions.push_back(x)
class Parser
{
public:
	Parser(Lexer* lexer);

	void start_parsing();

	const std::vector<Expression*>& expressions() { return m_expressions; };
private:
	Lexer* m_lexer;
	uint32_t m_current_token_index;
	Token* m_current_token;
	Token* m_current_no_consume_token;

	std::vector<Expression*> m_expressions;

	bool m_parsing_binary = false;
private:
	Expression* parse_expression(Token* token);
	BinaryExpression* parse_binary(Token* token);
	BinaryNumberExpression* parse_binary_number(Token* token);
	BinaryExpressionType parse_binary_op();
	NumberExpression* parse_number(Token* token);
	
	void parse_finished_expression(Expression* expression);
	void parse_finished_number_expression(NumberExpression* number);
	void parse_finished_binary_op_expression(BinaryExpression* binary);
	void parse_finished_binary_expression(BinaryExpression* binary);
	
	bool is_token_number(Token* token) { TokenType t = token->type; return t == TOKEN_INT || t == TOKEN_FLOAT || t == TOKEN_DOUBLE; };
	bool is_token_binary(Token* token) { TokenType t = token->type; return t == TOKEN_DIVIDE || t == TOKEN_MINUS || t == TOKEN_PLUS || t == TOKEN_MULTIPLY; }

	Token* peek_not_consume() { return m_current_no_consume_token = m_lexer->peek_no_consume(); };
	Token* peek_next() { return m_current_token = m_lexer->peek_next(); };
};