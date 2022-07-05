#include <assert.h>
#include <stdlib.h>

#include "ast.h"
#include "stack.h"

void stack_push(stack_t** stack, void* value)
{
	assert(stack != NULL);

	stack_t* new_item = malloc(sizeof(stack_t));
	assert(new_item != NULL);

	new_item->value = value;
	new_item->next = *stack;
	*stack = new_item;
}

void* stack_pop(stack_t** stack)
{
	assert(stack != NULL);
	assert(*stack != NULL);

	void* value = NULL;
	stack_t* removed_item = *stack;

	*stack = removed_item->next;
	value = removed_item->value;
	free(removed_item);

	return value;
}

ast_t* stack_to_tree(stack_t** stack)
{
	assert(stack != NULL);

	if (*stack == NULL) return NULL;

	ast_t* current = (ast_t*)stack_pop(stack);
	
	if (current->type == AST_UNARY && current->unary.op == AST_PARENTHESIS) {
		ast_t* tmp = current->unary.operand;
		free(current);
		current = tmp;
	}
	if (current->type = AST_BINARY) {
		if (current->binary.right == NULL) {
			current->binary.right = stack_to_tree(stack);
		}
		if (current->binary.left == NULL) {
			current->binary.left = stack_to_tree(stack);
		}
	}
	return current;
}
