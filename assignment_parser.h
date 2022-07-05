#ifndef ASSIGNMENT_PARSER
#define ASSIGNMENT_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* assignment_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** locla_table);

#endif /* ifndef ASSIGNMENT_PARSER */
