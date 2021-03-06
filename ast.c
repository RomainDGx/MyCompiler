#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "type.h"

static ast_t* ast_init(ast_node_type_e type)
{
	ast_t* ast = malloc(sizeof(ast_t));
	assert(ast != NULL);
	ast->type = type;
	return ast;
}

ast_t* ast_new_integer(long val)
{
	ast_t* ast = ast_init(AST_INTEGER);
	ast->integer = val;
	return ast;
}

ast_t* ast_new_boolean(bool val)
{
	ast_t* ast = ast_init(AST_BOOLEAN);
	ast->boolean = val;
	return ast;
}

ast_t* ast_new_unary(ast_unary_e op, ast_t* operand)
{
	ast_t* ast = ast_init(AST_UNARY);
	ast->unary.op = op;
	ast->unary.operand = operand;
	return ast;
}

ast_t* ast_new_binary(ast_binary_e op, ast_t* left, ast_t* right)
{
	ast_t* ast = ast_init(AST_BINARY);
	ast->binary.op = op;
	ast->binary.left = left;
	ast->binary.right = right;
	return ast;
}

ast_t* ast_new_function(char* name, type_e return_type, ast_list_t* params, ast_list_t* stmts)
{
	ast_t* ast = ast_init(AST_FUNCTION);
	ast->function.name = name;
	ast->function.return_type = return_type;
	ast->function.params = params;
	ast->function.statments = stmts;
	return ast;
}

ast_t* ast_new_fncall(char* name, ast_list_t* args)
{
	ast_t* ast = ast_init(AST_FNCALL);
	ast->call.name = name;
	ast->call.args = args;
	return ast;
}

ast_t* ast_new_variable(char* name, type_e type)
{
	ast_t* ast = ast_init(AST_VARIABLE);
	ast->var.name = name;
	ast->var.type = type;
	return ast;
}

ast_t* ast_new_condition(ast_t* condition, ast_t* valid, ast_t* invalid)
{
	ast_t* ast = ast_init(AST_CONDITION);
	ast->branch.condition = condition;
	ast->branch.valid = valid;
	ast->branch.invalid = invalid;
	return ast;
}

ast_t* ast_new_loop(ast_t* condition, ast_t* stmt)
{
	ast_t* ast = ast_init(AST_LOOP);
	ast->loop.condition = condition;
	ast->loop.statement = stmt;
	return ast;
}

ast_t* ast_new_declaration(ast_t* lvalue, ast_t* rvalue)
{
	ast_t* ast = ast_init(AST_DECLARATION);
	ast->declaration.lvalue = lvalue;
	ast->declaration.rvalue = rvalue;
	return ast;
}

ast_t* ast_new_assignment(ast_t* lvalue, ast_t* rvalue)
{
	ast_t* ast = ast_init(AST_ASSIGNMENT);
	ast->assigment.lvalue = lvalue;
	ast->assigment.rvalue = rvalue;
	return ast;
}

ast_t* ast_new_comp_stmt(ast_list_t* stmts)
{
	ast_t* ast = ast_init(AST_COMPOUND_STATEMENT);
	ast->compound_statements.statements = stmts;
	return ast;
}

ast_t* ast_new_return(ast_t* expr)
{
	ast_t* ast = ast_init(AST_RETURN);
	ast->ret.expression = expr;
	return ast;
}

ast_list_t* ast_list_new_node(ast_t* elem)
{
	ast_list_t* node = (ast_list_t*)malloc(sizeof(ast_list_t));
	assert(node != NULL);
	node->value = elem;
	node->next = NULL;
	return node;
}

ast_list_t* ast_list_add(ast_list_t* list, ast_t* elem)
{
	if (list == NULL)
	{
		return ast_list_new_node(elem);
	}

	ast_list_t* current = list;

	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = ast_list_new_node(elem);

	return list;
}
