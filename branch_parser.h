#ifndef BRANCH_PARSER
#define BRANCH_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* branch_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndif BRANCH_PARSER */
