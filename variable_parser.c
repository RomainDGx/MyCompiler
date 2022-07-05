#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"
#include "lexer.h"
#include "utils.h"
#include "type.h"
#include "variable_parser.h"

ast_t* variable_parser(buffer_t* buffer)
{
	char* type_name = lexer_getalphanum(buffer);
	type_e type = parse_type(type_name, buffer);
	free(type_name);

	char* variable_name = lexer_getalphanum(buffer);
	
	if (*variable_name == '\0')
	{
		parse_error("Invalid token, expected variable name.", buffer, 1);
	}

	return ast_new_variable(variable_name, type);
}
