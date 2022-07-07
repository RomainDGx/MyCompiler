#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "expression_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "utils.h"

static ast_t* parse_negate(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	if (buf_getchar(buffer) != '!')
	{
		parse_error("Invalid token, expected '!'.", buffer, 1);
	}
	if (buf_getchar_rollback(buffer) == '!')
	{
		return ast_new_unary(AST_NEGATE, parse_negate(buffer, global_table, local_table));
	}

	return ast_new_unary(AST_NEGATE, expression_parser(buffer, local_table, global_table));

	char* lexem = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*local_table, lexem);

	ast_t* ast = NULL;

	if (strcmp(lexem, "vrai") == 0)
	{
		ast = ast_new_boolean(true);
	}
	else if (strcmp(lexem, "faux") == 0)
	{
		ast = ast_new_boolean(false);
	}
	else if (symbol != NULL)
	{
		if (symbol->type != TYPE_BOOLEAN)
		{
			parse_error("Invalid variable type, expected boolean.", buffer, 1);
		}
		ast = symbol->attributes;
	}
	else
	{
		parse_error("Invalid token.", buffer, 1);
	}
	free(lexem);

	return ast_new_unary(AST_NEGATE, ast);
}

static ast_t* parse_left_increment(buffer_t* buffer, symbol_t** local_table)
{
	if (buf_getchar(buffer) != '+' || buf_getchar(buffer) != '+')
	{
		parse_error("Invalid token, expeted ++ operator.", buffer, 1);
	}
	if (!isalpha(buf_getchar_rollback(buffer)))
	{
		parse_error("Invalid token, expected variable.", buffer, 1);
	}

	char* variable_name = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*local_table, variable_name);
	free(variable_name);
	if (symbol == NULL)
	{
		parse_error("Cannot find variable name.", buffer, 1);
	}
	if (symbol->type != TYPE_INTEGER)
	{
		parse_error("Invalid variable type, expected integer.", buffer, 1);
	}

	return ast_new_unary(AST_LEFT_INCREMENT, symbol->attributes);
}

static ast_t* parse_right_increment(buffer_t* buffer, symbol_t* symbol)
{
	if (symbol == NULL)
	{
		parse_error("Cannot find variable name.", buffer, 1);
	}
	if (symbol->type != TYPE_INTEGER)
	{
		parse_error("Invalid variable type, expected integer.", buffer, 1);
	}
	if (buf_getchar(buffer) != '+' || buf_getchar(buffer) != '+')
	{
		parse_error("Invalid token, expeted ++ operator.", buffer, 1);
	}

	return ast_new_unary(AST_RIGHT_INCREMENT, symbol->attributes);
}

static ast_t* parse_left_decrement(buffer_t* buffer, symbol_t** local_table)
{
	if (buf_getchar(buffer) != '-' || buf_getchar(buffer) != '-')
	{
		parse_error("Invalid token, expeted -- operator.", buffer, 1);
	}
	if (!isalpha(buf_getchar_rollback(buffer)))
	{
		parse_error("Invalid token, expected variable.", buffer, 1);
	}

	char* variable_name = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*local_table, variable_name);
	free(variable_name);
	if (symbol == NULL)
	{
		parse_error("Cannot find variable name.", buffer, 1);
	}
	if (symbol->type != TYPE_INTEGER)
	{
		parse_error("Invalid variable type, expected integer.", buffer, 1);
	}

	return ast_new_unary(AST_LEFT_DECREMENT, symbol->attributes);
}

static ast_t* parse_right_decrement(buffer_t* buffer, symbol_t* symbol)
{
	if (symbol == NULL)
	{
		parse_error("Cannot find variable name.", buffer, 1);
	}
	if (symbol->type != TYPE_INTEGER)
	{
		parse_error("Invalid variable type, expected integer.", buffer, 1);
	}
	if (buf_getchar(buffer) != '-' || buf_getchar(buffer) != '-')
	{
		parse_error("Invalid token, expeted -- operator.", buffer, 1);
	}

	return ast_new_unary(AST_RIGHT_DECREMENT, symbol->attributes);
}

static ast_t* parse_parenthesis(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	if (buf_getchar(buffer) != '(')
	{
		parse_error("Invalid token, expected '('.", buffer, 1);
	}
	
	ast_t* expression = expression_parser(buffer, global_table, local_table);

	if (buf_getchar(buffer) != ')')
	{
		parse_error("Invalid token, expected ')'.", buffer, 1);
	}

	return ast_new_unary(AST_PARENTHESIS, expression);
}

ast_t* unary_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char c = buf_getchar_rollback(buffer);

	if (c == '!')
	{
		return parse_negate(buffer, global_table, local_table);
	}
	if (c == '+')
	{
		return parse_left_increment(buffer, local_table);
	}
	if (c == '-')
	{
		return parse_left_decrement(buffer, local_table);
	}
	if (c == '(')
	{
		return parse_parenthesis(buffer, global_table, local_table);
	}
	if (!isalpha(c))
	{
		parse_error("Invalid token.", buffer, 1);
	}

	char* variable_name = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*local_table, variable_name);
	free(variable_name);
	if (symbol == NULL)
	{
		parse_error("Cannot find variable name.", buffer, 1);
	}

	c = buf_getchar_rollback(buffer);

	if (c == '+')
	{
		return parse_right_increment(buffer, symbol);
	}
	if (c == '-')
	{
		return parse_right_decrement(buffer, symbol);
	}
	
	parse_error("Invalid token, expected increment or decrement token.", buffer, 1);
}
