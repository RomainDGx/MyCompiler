#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "expression_parser.h"
#include "lexer.h"
#include "return_parser.h"
#include "symbol.h"
#include "utils.h"

ast_t* return_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* return_keyword = lexer_getalphanum(buffer);
	if (strcmp(return_keyword, "retourner") != 0)
	{
		parse_error("Invalid keyword name, expected 'retourner'.", buffer, 1);
	}
	free(return_keyword);

	ast_t* expression = expression_parser(buffer, global_table, local_table);

	if (buf_getchar_after_blank(buffer) != ';')
	{
		parse_error("Invalid token, expected ';'.", buffer, 1);
	}

	return ast_new_return(expression);
}
