#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "function_parser.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"
#include "utils.h"

ast_list_t* parser(buffer_t* buffer, symbol_t** global_table)
{
	ast_list_t* functions = NULL;

	while (!buf_eof(buffer))
	{
		ast_t* function = function_parser(buffer, global_table);

		functions = ast_list_add(functions, function);
	}

	return functions;
}
