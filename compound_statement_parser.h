#ifndef COMPOUND_STATEMENT_PARSER
#define COMPOUND_STATEMENT_PARSER

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_t* compound_statement_parser(buffer_t* buffer, symbol_t** global_table, symbol_t** local_table);

#endif /* ifndif COMPOUND_STATEMENT_PARSER */
