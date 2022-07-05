#ifndef FUNCTION_PARSER
#define FUNCTION_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* function_parser(buffer_t* buffer, symbol_t** global_table);

#endif /* ifndif FUNCTION_PARSER */
