#ifndef UNARY_PARSER
#define UNARY_PARSER

#include "buffer.h"
#include "symbol.h"

ast_t* unary_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndef UNARY_PARSER */
