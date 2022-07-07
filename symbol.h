#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"

typedef struct symbol_t {
	char* name;
	type_e type;
	ast_t* attributes;
	struct symbol_t* next;
} symbol_t;

symbol_t* sym_new(char* name, type_e type, ast_t* attributes);
void sym_delete(symbol_t* sym);
void sym_remove(symbol_t** table, symbol_t* sym);
void sym_add(symbol_t** table, symbol_t* sym);
symbol_t* sym_search(symbol_t* table, char* name);
void sym_clear(symbol_t* table);

#endif /* ifndif SYMBOL_H */
