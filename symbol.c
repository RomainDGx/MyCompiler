#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

symbol_t* sym_new(char* name, type_e type, ast_t* attributes)
{
	symbol_t* symbol = malloc(sizeof(symbol_t));
	assert(symbol != NULL);

	symbol->name = name;
	symbol->type = type;
	symbol->attributes = attributes;
	symbol->next = NULL;

	return symbol;
}

void sym_delete(symbol_t* sym)
{
	while (sym != NULL)
	{
		symbol_t* next = sym->next;
		free(sym);
		sym = next;
	}
}

void sym_remove(symbol_t** table, symbol_t* sym)
{
	if (*table == NULL || sym == NULL) return;

	if (*table == sym)
	{
		*table = sym->next;
		free(sym);
		return;
	}

	symbol_t* current = *table;

	while (current->next != NULL)
	{
		if (current->next == sym)
		{
			current->next = sym->next;
			free(sym);
			return;
		}
		current = current->next;
	}
}

void sym_add(symbol_t** table, symbol_t* sym)
{
	if (*table == NULL)
	{
		*table = sym;
	}
	else
	{
		symbol_t* current = *table;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = sym;
	}
}

symbol_t* sym_search(symbol_t* table, char* name)
{
	while (table != NULL)
	{
		if (strcmp(table->name, name) == 0)
		{
			return table;
		}
		table = table->next;
	}
	return NULL;
}

void sym_clear(symbol_t* table)
{
	while (table != NULL)
	{
		symbol_t* next = table->next;
		free(table);
		table = next;
	}
}
