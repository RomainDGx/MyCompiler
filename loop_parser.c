#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "compound_statement_parser.h"
#include "expression_parser.h"
#include "lexer.h"
#include "loop_parser.h"
#include "symbol.h"
#include "utils.h"

ast_t* loop_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* loop_keyword = lexer_getalphanum(buffer);
	if (strcmp(loop_keyword, "tantque") != 0)
	{
		parse_error("Invalid keyword, expected 'tantque'.", buffer, 1);
	}
	free(loop_keyword);

	if (buf_getchar_after_blank(buffer) != '(')
	{
		parse_error("Invalid token, expected '('.", buffer, 1);
	}

	ast_t* condition = expression_parser(buffer, global_table, local_table);

	if (buf_getchar_after_blank(buffer) != ')')
	{
		parse_error("Invalid token, expected ')'.", buffer, 1);
	}

	ast_t* statements = statement_parser(buffer, global_table, local_table);

	return ast_new_loop(condition, statements);
}

