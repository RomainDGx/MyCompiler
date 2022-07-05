#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "assignment_parser.h"
#include "ast.h"
#include "branch_parser.h"
#include "buffer.h"
#include "compound_statement_parser.h"
#include "declaration_parser.h"
#include "function_call_parser.h"
#include "lexer.h"
#include "loop_parser.h"
#include "return_parser.h"
#include "symbol.h"
#include "unary_statement_parser.h"
#include "utils.h"

ast_t* compound_statement_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	if (buf_getchar_after_blank(buffer) != '{')
	{
		parse_error("Invalid token, expected '{'.", buffer, 1);
	}

	ast_list_t* statements = NULL;

	while (true)
	{
		ast_t* ast = NULL;

		char c = buf_getchar_rollback(buffer);
		if (c == '}')
		{
			break;
		}
		else if (isalpha(c))
		{
			char* lexem = lexer_getalphanum_rollback(buffer);
			
			if (sym_search(*local_table, lexem) != NULL)
			{
				buf_lock(buffer);
				int pos = buffer->it;
				buf_forward(buffer, strlen(lexem));
				char c = buf_getchar_rollback(buffer);
				buf_rollback_and_unlock(buffer, buffer->it - pos);

				if (c == '=')
				{
					ast = assignment_parser(buffer, global_table, local_table);
				}
				else if (c == '+' || c == '-')
				{
					ast = unary_parser(buffer, global_table, local_table);

					if (buf_getchar_after_blank(buffer) != ';')
					{
						parse_error("Invalid token, expected ';'.", buffer, 1);
					}
				}
				else
				{
					parse_error("Invalid token.", buffer, 1);
				}
			}
			else if (strcmp(lexem, "si") == 0)
			{
				ast = branch_parser(buffer, global_table, local_table);
			}
			else if (strcmp(lexem, "entier") == 0 || strcmp(lexem, "booleen") == 0)
			{
				ast = declaration_parser(buffer, global_table, local_table);
			}
			else if (sym_search(*global_table, lexem) != NULL)
			{
				ast = function_call_parser(buffer, global_table, local_table);

				if (buf_getchar_after_blank(buffer) != ';')
				{
					parse_error("Invalid token, expected ';'.", buffer, 1);
				}
			}
			else if (strcmp(lexem, "tantque") == 0)
			{
				ast = loop_parser(buffer, global_table, local_table);
			}
			else if (strcmp(lexem, "retourner") == 0)
			{
				ast = return_parser(buffer, global_table, local_table);
			}
			else
			{
				parse_error("Invalid token.", buffer, 1);
			}
			free(lexem);
		}
		else if (c == '+' || c == '-')
		{
			ast = unary_parser(buffer, global_table, local_table);

			if (buf_getchar_after_blank(buffer) != ';')
			{
				parse_error("Invalid token, expected ';'.", buffer, 1);
			}
		}
		else
		{
			parse_error("Invalid token.", buffer, 1);
		}

		assert(ast != NULL);
		statements = ast_list_add(statements, ast);
	}

	if (buf_getchar_after_blank(buffer) != '}')
	{
		parse_error("Invalid token, expected '}'.", buffer, 1);
	}

	return ast_new_comp_stmt(statements);
}
