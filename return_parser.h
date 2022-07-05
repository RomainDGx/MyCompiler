#ifndef RETURN_PARSER
#define RETURN_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* return_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif  /* ifndif RETURN_PARSER */
