#ifndef STACK_H
#define STACK_H

#include "ast.h"

typedef struct stack_t {
	void* value;
	struct stack_t* next;
} stack_t;

void stack_push(stack_t** stack, void* value);
void* stack_pop(stack_t** stack);

ast_t* stack_to_tree(stack_t** stack);

#endif /* STACK_H */
