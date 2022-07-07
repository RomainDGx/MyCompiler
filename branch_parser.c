#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "branch_parser.h"
#include "buffer.h"
#include "compound_statement_parser.h"
#include "expression_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "utils.h"

ast_t* branch_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* keyword = lexer_getalphanum(buffer);
	if (strcmp(keyword, "si") != 0)
	{
		parse_error("Invalid token type, expected 'si'.", buffer, 1);
	}

	if (buf_getchar_after_blank(buffer) != '(')
	{
		parse_error("Invalid token, expected '('.", buffer, 1);
	}

	ast_t* condition = expression_parser(buffer, global_table, local_table);
	assert(condition != NULL);

	if (buf_getchar_after_blank(buffer) != ')')
	{
		parse_error("Invalid token, expected ')'.", buffer, 1);
	}

	ast_t* statements = statement_parser(buffer, global_table, local_table);
	assert(statements != NULL);

	ast_t* else_statement = NULL;

	keyword = lexer_getalphanum_rollback(buffer);

	if (strcmp(keyword, "sinon") == 0)
	{
		buf_forward(buffer, 5);

		else_statement = statement_parser(buffer, global_table, local_table);
	}
	free(keyword);

	return ast_new_condition(condition, statements, else_statement);
}
