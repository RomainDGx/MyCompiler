#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"
#include "type.h"
#include "utils.h"

static ast_list_t* parse_parameters(buffer_t* buffer, symbol_t** table)
{
	ast_list_t* list = NULL;

	buf_lock(buffer);

	char c = buf_getchar_after_blank(buffer);

	if (c != '(')
	{
		parse_error("Invalid token, expected '('.", buffer, 1);
	}

	c = buf_getchar_after_blank(buffer);
	if (c == ')')
	{
		// TODO: Should not rollback?
		//buf_rollback_and_unlock(buffer, 1);
		return list;
	}

	buf_rollback_and_unlock(buffer, 1);

	while (true)
	{
		char* type_name = lexer_getalphanum(buffer);
		type_e type = parse_type(type_name, buffer);

		char* name = lexer_getalphanum(buffer);

		if (sym_search(*table, name) == NULL)
		{
			sym_add(table, sym_new(name, type, NULL));
		}

		list = ast_list_add(list, ast_new_variable(name, type));

		c = buf_getchar_after_blank(buffer);

		if (c == ',')
		{
			continue;
		}
		else if (c == ')')
		{
			break;
		}
		else
		{
			parse_error("Invalid token, expected ',' or ')'", buffer, 1);
		}
	}

	return list;
}

static type_e parse_return_type(buffer_t* buffer)
{
	char c = buf_getchar_after_blank(buffer);
	if (c != ':')
	{
		parse_error("Invalid token, expected ':'", buffer, 1);
	}

	char* return_type = lexer_getalphanum(buffer);

	return parse_type(return_type, buffer);
}

static ast_list_t* parse_function_statements(buffer_t* buffer, symbol_t** table)
{
	ast_list_t* list = NULL;

	buf_lock(buffer);

	char c = buf_getchar_after_blank(buffer);

	if (c != '{')
	{
		parse_error("Invalid token, expected '{'", buffer, 1);
	}

	buf_unlock(buffer);

	while (true)
	{
		buf_lock(buffer);
		c = buf_getchar_after_blank(buffer);
		if (c == '}')
		{
			// Todo: Should not rollback?
			//buf_rollback_and_unlock(buffer, 1);
			return list;
		}
		buf_rollback_and_unlock(buffer, 1);

		char* type_name = lexer_getalphanum(buffer);
		type_e type = parse_type(type_name, buffer);

		char* name = lexer_getalphanum(buffer);

		if (sym_search(*table, name) == NULL)
		{
			sym_add(table, sym_new(name, type, NULL));
		}

		list = ast_list_add(list, ast_new_variable(name, type));

		c = buf_getchar_after_blank(buffer);
		if (c == ';')
		{
			continue;
		}
		else
		{
			parse_error("Invalid token, expected ';'", buffer, 1);
		}
	}
	return list;
}

static type_e parse_type(char* name, buffer_t* buffer)
{
	if (strcmp(name, "entier") == 0)
	{
		return TYPE_INTEGER;
	}
	if (strcmp(name, "booleen") == 0)
	{
		return TYPE_BOOLEAN;
	}
	if (strcmp(name, "rien") == 0)
	{
		return TYPE_VOID;
	}
	parse_error("Invalid type", buffer, 1);
}

ast_t* parse_function(buffer_t* buffer, symbol_t** table)
{
	char* name = lexer_getalphanum(buffer);
	ast_list_t* parameters = parse_parameters(buffer, table);
	type_e return_type = parse_return_type(buffer);
	ast_list_t* statements = parse_function_statements(buffer, table);
	return ast_new_function(name, return_type, parameters, statements);
}

ast_list_t* parser(buffer_t* buffer, symbol_t** table)
{
	ast_list_t* list = NULL;

	buf_lock(buffer);

	while (!buf_eof(buffer))
	{
		buf_unlock(buffer);

		char* function_keyword = lexer_getalphanum(buffer);

		if (strcmp(function_keyword, "fonction"))
		{
			parse_error("Invalid token, expected 'fonction' keyword", buffer, 1);
		}

		ast_t* function = parse_function(buffer, table);

		list = ast_list_add(list, function);

		buf_lock(buffer);
	}

	return list;
}
