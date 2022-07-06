#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "expression_parser.h"
#include "function_call_parser.h"
#include "lexer.h"
#include "stack.h"
#include "symbol.h"
#include "unary_statement_parser.h"
#include "utils.h"

static ast_t* expression_value_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char c = buf_getchar_rollback(buffer);

	if (c == ')' || c == ';' || c == ',')
	{
		return NULL;
	}
	else if (isalpha(c))
	{
		ast_t* ast = NULL;
		char* lexem = lexer_getalphanum_rollback(buffer);
		symbol_t* symbol = sym_search(*local_table, lexem);
		if (symbol != NULL)
		{
			ast = symbol->attributes;
			buf_forward(buffer, strlen(ast->var.name));
		}
		else if ((symbol = sym_search(*global_table, lexem)) != NULL)
		{
			ast = function_call_parser(buffer, global_table, local_table);
		}
		else if (strcmp(lexem, "vrai") == 0)
		{
			ast = ast_new_boolean(true);
			buf_forward(buffer, 4);
		}
		else if (strcmp(lexem, "faux") == 0)
		{
			ast = ast_new_boolean(false);
			buf_forward(buffer, 4);
		}
		else
		{
			// TODO: Manage incrementation and decrementation...
			parse_error("Invalid token.", buffer, 1);
		}
		free(lexem);

		return ast;
	}
	else if (isdigit(c))
	{
		return ast_new_integer(lexer_getnumber(buffer));
	}
	else if (c == '!')
	{
		return unary_parser(buffer, global_table, local_table);
	}
	else if (c == '*')
	{
		buf_forward(buffer, 1);
		return ast_new_binary(AST_MULTIPLICATION, NULL, NULL);
	}
	else if (c == '/')
	{
		buf_forward(buffer, 1);
		return ast_new_binary(AST_DIVISION, NULL, NULL);
	}
	else if (c == '+')
	{
		buf_forward(buffer, 1);
		return ast_new_binary(AST_ADDITION, NULL, NULL);
	}
	else if (c == '-')
	{
		// TODO: Check if is negative number...
		buf_forward(buffer, 1);
		return ast_new_binary(AST_SUBSTRACTION, NULL, NULL);
	}
	else if (c == '<')
	{
		buf_lock(buffer);
		buf_forward(buffer, 1);

		if (buf_getchar(buffer) == '=')
		{
			buf_unlock(buffer);
			return ast_new_binary(AST_LESS_EQUAL, NULL, NULL);
		}
		else
		{
			buf_rollback_and_unlock(buffer, 1);
			return ast_new_binary(AST_LESS, NULL, NULL);
		}
	}
	else if (c == '>')
	{
		buf_lock(buffer);
		buf_forward(buffer, 1);

		if (buf_getchar(buffer) == '=')
		{
			buf_unlock(buffer);
			return ast_new_binary(AST_GRATER_EQUAL, NULL, NULL);
		}
		else
		{
			buf_rollback_and_unlock(buffer, 1);
			return ast_new_binary(AST_GREATER, NULL, NULL);
		}
	}
	else if (c == '&')
	{
		char* token = malloc(3);
		buf_getnchar(buffer, token, 2);
		token[2] = '\0';
		assert(token != NULL);

		if (strcmp(token, "&&") != 0)
		{
			parse_error("Invalid toke, expected '&&'.", buffer, 1);
		}
		free(token);

		return ast_new_binary(AST_AND, NULL, NULL);
	}
	else if (c == '|')
	{
		char* token = (char*)malloc(3);
		buf_getnchar(buffer, token, 2);
		token[2] = '\0';
		assert(token != NULL);

		if (strcmp(token, "||") != 0)
		{
			parse_error("Invalid token, expected '||'.", buffer, 1);
		}
		free(token);

		return ast_new_binary(AST_OR, NULL, NULL);
	}
	else if (c == '(')
	{
		return unary_parser(buffer, global_table, local_table);
	}
	else
	{
		parse_error("Invalid token.", buffer, 1);
	}
}

static int get_symbol_priority(ast_t* symbol)
{
	if (symbol == NULL) return 5;

	switch (symbol->type)
	{
		case AST_INTEGER:
		case AST_BOOLEAN:
		case AST_VARIABLE:
		case AST_FNCALL:
		case AST_UNARY:
			return 0;

		case AST_BINARY:
			switch (symbol->binary.op)
			{
				case AST_MULTIPLICATION:
				case AST_DIVISION:
					return 1;

				case AST_ADDITION:
				case AST_SUBSTRACTION:
					return 2;

				case AST_LESS:
				case AST_LESS_EQUAL:
				case AST_EQUAL:
				case AST_NOT_EQUAL:
				case AST_GRATER_EQUAL:
				case AST_GREATER:
					return 3;

				case AST_AND:
				case AST_OR:
					return 4;

				default:
					// TODO: Throws an error "Invalod binary operator type"...
					break;
			}
			break;

		default:
			// TODO: Throws an error "Invalid ast type"...
			break;
	}
}

static bool left_is_prior(ast_t* left, ast_t* right)
{
	// 0: AST_INTEGER AST_BOOLEAN AST_VARIABLE AST_FNCALL AST_UNARY
	// 1: AST_MULTIPLICATION AST_DIVISION
	// 2: AST_ADDITION AST_SUBSTRACTION
	// 3: AST_LESS AST_LESS_EQUAL AST_EQUAL AST_NOT_EQUAL AST_GRATER_EQUAL AST_GREATER
	// 4: AST_AND AST_OR
	// 5: NULL

	int left_priority = get_symbol_priority(left);
	int right_priority = get_symbol_priority(right);

	if (left_priority == 0 && right_priority == 0)
	{
		// TODO: Throws an error when left and right are priority 0
	}

	if (left_priority == 5)
	{
		return false;
	}

	return left_priority <= right_priority;
}

ast_t* expression_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	stack_t* output = NULL;

	ast_t* i = expression_value_parser(buffer, global_table, local_table);

	stack_t* stack = NULL;
	stack_push(&stack, NULL);

	while (true)
	{
		if (i == NULL && (ast_t*)((*stack).value) == NULL)
		{
			return stack_to_tree(&output);
		}
		else
		{
			ast_t* a = (ast_t*)((*stack).value);
			ast_t* b = i;

			if (!left_is_prior(a, b))
			{
				stack_push(&stack, b);
				i = expression_value_parser(buffer, global_table, local_table);
			}
			else
			{
				do
				{
					stack_push(&output, (ast_t*)stack_pop(&stack));
				}
				while (left_is_prior((ast_t*)(*stack).value, (ast_t*)(*output).value));
			}
		}
	}

	return stack_to_tree(&stack);
}
