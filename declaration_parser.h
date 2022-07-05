#ifndef DECLARATION_PARSER
#define DECLARATION_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* declaration_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndef DECLARATION_PARSER */
