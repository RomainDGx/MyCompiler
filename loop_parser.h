#ifndef LOOP_PARSER
#define LOOP_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* loop_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndif LOOP_PARSER */
