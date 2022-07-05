#include <stdlib.h>

#include "assignment_parser.h"
#include "ast.h"
#include "buffer.h"
#include "expression_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "utils.h"

ast_t* assignment_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* variable_name = lexer_getalphanum(buffer);
	symbol_t* symbol = sym_search(*local_table, variable_name);
	free(variable_name);
	if (symbol == NULL)
	{
		parse_error("Variable is not defined.", buffer, 1);
	}

	char c = buf_getchar_after_blank(buffer);
	if (c != '=')
	{
		parse_error("Invalid token, expected '='.", buffer, 1);
	}

	ast_t* expression = expression_parser(buffer, global_table, local_table);

	// TODO: Check expression result type is equal to variable (symbol->attriutes) type.

	if (buf_getchar_after_blank(buffer) != ';')
	{
		parse_error("Invalid token, expected ';'.", buffer, 1);
	}

	return ast_new_assignment(symbol->attributes, expression);
}
