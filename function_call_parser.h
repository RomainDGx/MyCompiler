#ifndef FUNCTION_CALL_STATEMENT_PARSER
#define FUNCTION_CALL_STATEMENT_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* function_call_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndif FUNCTION_CALL_STATEMENT_PARSER */
