#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "expression_parser.h"
#include "function_call_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "utils.h"

ast_t* function_call_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* function_name = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*global_table, function_name);
	free(function_name);
	if (symbol == NULL)
	{
		parse_error("Cannot find function with this name.", buffer, 1);
	}

	if (buf_getchar(buffer) != '(')
	{
		parse_error("Invalid token, exepected '('.", buffer, 1);
	}

	assert(symbol != NULL && symbol->attributes != NULL);
	ast_list_t* exepcted_params = symbol->attributes->function.params;
	ast_list_t* params = NULL;

	buf_lock(buffer);
	if (buf_getchar_after_blank(buffer) == ')')
	{
		buf_unlock(buffer);
		goto last_check;
	}

	buf_rollback_and_unlock(buffer, 1);

	while (true)
	{
		if (exepcted_params == NULL)
		{
			parse_error("Too many arguments.", buffer, 1);
		}

		// TODO: Replace by expression_parser...

		ast_t* ast = NULL;
		char c = buf_getchar_rollback(buffer);

		if (isdigit(c) || c == '-')
		{
			ast = ast_new_integer(lexer_getnumber(buffer));
		}
		else if (isalpha(c))
		{
			char* lexem = lexer_getalphanum(buffer);

			if (strcmp(lexem, "vrai") == 0)
			{
				ast = ast_new_boolean(true);
			}
			else if (strcmp(lexem, "faux") == 0)
			{
				ast = ast_new_boolean(false);
			}
			else
			{
				symbol_t* symbol = sym_search(*local_table, lexem);
				if (symbol == NULL)
				{
					parse_error("Invalid argument.", buffer, 1);
				}
				ast = symbol->attributes;
			}

			free(lexem);
		}
		else
		{
			parse_error("Invalid argument", buffer, 1);
		}

		// ----------

		assert(ast != NULL);
		assert(exepcted_params != NULL && exepcted_params->value != NULL && exepcted_params->value->type == AST_VARIABLE);
		
		// TODO: Compare AST types and TYPE type...
		//if (exepcted_params->value->var.type != ast->type)
		//{
		//	parse_error("Invalid error type.", buffer, 1);
		//}

		params = ast_list_add(params, ast);
	
		exepcted_params = exepcted_params->next;

		c = buf_getchar(buffer);

		if (c == ',')
		{
			continue;
		}
		if (c == ')')
		{
			break;
		}
		else
		{
			parse_error("Invalid token, expected ',' or ')' token.", buffer, 1);
		}
	}

	last_check:

	if (exepcted_params != NULL)
	{
		parse_error("Expected more arguments.", buffer, 1);
	}

	return ast_new_fncall(symbol->attributes->function.name, params);
}
