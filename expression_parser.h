#ifndef EXPRESSION_PARSER
#define EXPRESSION_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* expression_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndef EXPRESSION_PARSER */
