#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "buffer.h"
#include "symbol.h"

ast_list_t* parser(buffer_t* buffer, symbol_t** table);

#endif /* ifndef PARSER_H */
