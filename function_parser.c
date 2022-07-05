#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "compound_statement_parser.h"
#include "function_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "type.h"
#include "utils.h"
#include "variable_parser.h"

static ast_list_t* parse_parameters(buffer_t* buffer, symbol_t** local_table)
{
	ast_list_t* parameters = NULL;

	if (buf_getchar_after_blank(buffer) != '(')
	{
		parse_error("Invalid token, expected '('.", buffer, 1);
	}

	buf_lock(buffer);

	if (buf_getchar(buffer) == ')')
	{
		buf_unlock(buffer);
		return parameters;
	}

	buf_rollback_and_unlock(buffer, 1);

	while (true)
	{
		ast_t* variable = variable_parser(buffer);
		assert(variable != NULL);

		if (sym_search(*local_table, variable->var.name) != NULL)
		{
			parse_error("Parameter name is already used", buffer, 1);
		}
		sym_add(local_table, sym_new(variable->var.name, variable->var.type, variable));

		parameters = ast_list_add(parameters, variable);

		char c = buf_getchar_after_blank(buffer);

		if (c == ',')
		{
			continue;
		}
		else if (c == ')')
		{
			break;
		}
		parse_error("Invalid token, expected ',' or ')'", buffer, 1);
	}

	return parameters;
}

static type_e parse_return_type(buffer_t* buffer)
{
	if (buf_getchar_after_blank(buffer) != ':')
	{
		parse_error("Invalid token, expected ':'", buffer, 1);
	}

	char* return_type = lexer_getalphanum(buffer);
	type_e type = parse_type(return_type, buffer);
	free(return_type);

	return type;
}

ast_t* function_parser(buffer_t* buffer, symbol_t** global_table)
{
	char* function_keyword = lexer_getalphanum(buffer);
	if (strcmp(function_keyword, "fonction") != 0)
	{
		parse_error("Invalid function keyword, expected 'fonction'.", buffer, 1);
	}
	free(function_keyword);

	char* function_name = lexer_getalphanum(buffer);
	if (sym_search(*global_table, function_name) != NULL)
	{
		parse_error("A function has already been declared with this name.", buffer, 1);
	}

	symbol_t* local_table = NULL;

	ast_list_t* parameters = parse_parameters(buffer, &local_table);
	type_e return_type = parse_return_type(buffer);

	// Add function to the global symbol talbe to allow recursive call
	ast_t* function = ast_new_function(function_name, return_type, parameters, NULL);
	sym_add(global_table, sym_new(function->function.name, TYPE_FUNCTION, function));
	
	ast_t* statements = compound_statement_parser(buffer, global_table, &local_table);
	function->function.statments = statements->compound_statements.statements;
	free(statements);

	sym_clear(local_table);
	
	return function;
}
