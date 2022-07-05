#include "ast.h"
#include "buffer.h"
#include "declaration_parser.h"
#include "expression_parser.h"
#include "lexer.h"
#include "symbol.h"
#include "utils.h"

ast_t* declaration_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table)
{
	char* type_name = lexer_getalphanum(buffer);
	type_e type = parse_type(type_name, buffer);

	if (type == TYPE_VOID)
	{
		parse_error("Variable can't be a void type.", buffer, 1);
	}

	char* variable_name = lexer_getalphanum(buffer);
	ast_t* variable = ast_new_variable(variable_name, type);

	if (*(variable->var.name) == '\0')
	{
		parse_error("Expected a variable name.", buffer, 1);
	}

	if (sym_search(*local_table, variable->var.name) == NULL)
	{
		sym_add(local_table, sym_new(variable->var.name, variable->var.type, variable));
	}
	else
	{
		parse_error("This variable was already declared.", buffer, 1);
	}

	char c = buf_getchar_after_blank(buffer);

	if (c == ';')
	{
		return ast_new_declaration(variable, ast_new_void());
	}
	else if (c == '=')
	{
		ast_t* value = expression_parser(buffer, global_table, local_table);

		// TODO: check expected and received value type...

		c = buf_getchar_after_blank(buffer);
		if (c != ';')
		{
			parse_error("Invalod token, expected '='.", buffer, 1);
		}

		return ast_new_declaration(variable, value);
	}
	else
	{
		parse_error("Invalid token, expected ';'", buffer, 1);
	}
}
