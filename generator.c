#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "generator.h"
#include "type.h"

static char* create_tabs(int tabs)
{
	char* output = malloc(sizeof(char) * (tabs + 1));
	assert(output != NULL);

	for (int i = 0; i < tabs; i++)
	{
		output[i] = '\t';
	}
	output[tabs] = '\0';

	return output;
}

static void expression(FILE* file, ast_t* ast);
void compound_statement(FILE* file, ast_t* ast);

static void integer(FILE* file, ast_t* ast)
{
	fprintf(file, "%li", ast->integer);
}

static void boolean(FILE* file, ast_t* ast)
{
	fprintf(file, ast->boolean ? "true" : "false");
}

static void variable(FILE* file, ast_t* ast)
{
	fprintf(file, ast->var.name);
}

static void type(FILE* file, type_e type)
{
	switch (type)
	{
	case TYPE_INTEGER:
		fprintf(file, "long");
		break;

	case TYPE_BOOLEAN:
		fprintf(file, "bool");
		break;

	case TYPE_VOID:
		fprintf(file, "void");
		break;

	default:
		// TODO: Throws an error.
		break;
	}
}

static void type_variable(FILE* file, ast_t* ast)
{
	type(file, ast->var.type);
	fprintf(file, " ");
	variable(file, ast);
}

static void fn_call(FILE* file, ast_t* ast)
{
	fprintf(file, ast->call.name);
	fprintf(file, "(");

	ast_list_t* arg = ast->call.args;
	while (arg != NULL)
	{
		expression(file, arg->value);
		arg = arg->next;
		if (arg != NULL)
		{
			fprintf(file, ", ");
		}
	}

	fprintf(file, ")");
}

static void unary(FILE* file, ast_t* ast)
{
	switch (ast->unary.op)
	{
		case AST_LEFT_INCREMENT:
			fprintf(file, "++");
			expression(file, ast->unary.operand);
			break;

		case AST_RIGHT_INCREMENT:
			expression(file, ast->unary.operand);
			fprintf(file, "++");
			break;

		case AST_LEFT_DECREMENT:
			fprintf(file, "--");
			expression(file, ast->unary.operand);
			break;

		case AST_RIGHT_DECREMENT:
			expression(file, ast->unary.operand);
			fprintf(file, "--");
			break;

		case AST_NEGATE:
			fprintf(file, "!");
			expression(file, ast->unary.operand);
			break;

		case AST_PARENTHESIS:
			fprintf(file, "(");
			expression(file, ast->unary.operand);
			fprintf(file, ")");
			break;

		default:
			// Throws an error...
			break;
	}
}

static void binary(FILE* file, ast_t* ast)
{
	expression(file, ast->binary.left);
	
	switch (ast->binary.op)
	{
		case AST_MULTIPLICATION: fprintf(file, " * " ); break;
		case AST_DIVISION      : fprintf(file, " / " ); break;
		case AST_ADDITION      : fprintf(file, " + " ); break;
		case AST_SUBSTRACTION  : fprintf(file, " - " ); break;
		case AST_LESS          : fprintf(file, " < " ); break;
		case AST_LESS_EQUAL    : fprintf(file, " <= "); break;
		case AST_EQUAL         : fprintf(file, " == "); break;
		case AST_NOT_EQUAL     : fprintf(file, " != "); break;
		case AST_GREATER_EQUAL : fprintf(file, " >= "); break;
		case AST_GREATER       : fprintf(file, " > " ); break;
		case AST_AND           : fprintf(file, " && "); break;
		case AST_OR            : fprintf(file, " || "); break;

		default:
			// Throws an error...
			break;
	}

	expression(file, ast->binary.right);
}

static void ret(FILE* file, ast_t* ast)
{
	fprintf(file, "return ");
	expression(file, ast->ret.expression);
	fprintf(file, ";");
}

static void assignment(FILE* file, ast_t* ast)
{
	variable(file, ast->assigment.lvalue);
	fprintf(file, " = ");
	expression(file, ast->assigment.rvalue);
	fprintf(file, ";");
}

static void declaration(FILE* file, ast_t* ast)
{
	type_variable(file, ast->declaration.lvalue);

	if (ast->declaration.rvalue != NULL)
	{
		fprintf(file, " = ");
		expression(file, ast->declaration.rvalue);
	}

	fprintf(file, ";");
}

static void expression(FILE* file, ast_t* ast)
{
	switch (ast->type)
	{
		case AST_INTEGER:
			integer(file, ast);
			break;

		case AST_BOOLEAN:
			boolean(file, ast);
			break;

		case AST_VARIABLE:
			variable(file, ast);
			break;

		case AST_FNCALL:
			fn_call(file, ast);
			break;

		case AST_UNARY:
			unary(file, ast);
			break;

		case AST_BINARY:
			binary(file, ast);
			break;

		default:
			// Throws an error...
			break;
	}
}

static void loop(FILE* file, ast_t* ast, int tabs)
{
	fprintf(file, "while (");
	expression(file, ast->loop.condition);
	fprintf(file, ")\n");
	compound_statement(file, ast->loop.statement, tabs);
}

static void branch(FILE* file, ast_t* ast, int tabs)
{
	fprintf(file, "if (");
	expression(file, ast->branch.condition);
	fprintf(file, ")\n");
	compound_statement(file, ast->branch.valid, tabs);

	if (ast->branch.invalid != NULL)
	{
		char* strtabs = create_tabs(tabs);
		fprintf(file, "%selse", strtabs);
		free(strtabs);

		if (ast->branch.invalid->type == AST_CONDITION)
		{
			fprintf(file, " ");
			branch(file, ast->branch.invalid, tabs);
		}
		else if (ast->branch.invalid->type == AST_COMPOUND_STATEMENT)
		{
			fprintf(file, "\n");
			compound_statement(file, ast->branch.invalid, tabs);
		}
	}
}

static void compound_statement(FILE* file, ast_t* ast, int tabs)
{
	char* local_tabs = create_tabs(tabs);

	char* inside_tabs = create_tabs(tabs + 1);
		
	fprintf(file, "%s{\n", local_tabs);

	ast_list_t* statement = ast->compound_statements.statements;
	while (statement != NULL)
	{
		fprintf(file, inside_tabs);

		switch (statement->value->type)
		{
			case AST_FNCALL:
				fn_call(file, statement->value);
				fprintf(file, ";");
				break;

			case AST_UNARY:
				unary(file, statement->value);
				fprintf(file, ";");
				break;

			case AST_ASSIGNMENT:
				assignment(file, statement->value);
				break;

			case AST_DECLARATION:
				declaration(file, statement->value);
				break;

			case AST_RETURN:
				ret(file, statement->value);
				break;

			case AST_LOOP:
				loop(file, statement->value, tabs + 1);
				break;

			case AST_CONDITION:
				branch(file, statement->value, tabs + 1);
				break;

			default:
				break;
		}
		fprintf(file, "\n");

		statement = statement->next;
	}

	fprintf(file, "%s}\n", local_tabs);

	free(local_tabs);
	free(inside_tabs);
}

static void function(FILE* file, ast_t* ast)
{
	type(file, ast->function.return_type);
	fprintf(file, " %s(", ast->function.name);

	ast_list_t* param = ast->function.params;
	while (param != NULL)
	{
		type_variable(file, param->value);

		param = param->next;

		if (param != NULL)
		{
			fprintf(file, ", ");
		}
	}

	fprintf(file, ")\n");

	ast_t* statements = ast_new_comp_stmt(ast->function.statments);
	compound_statement(file, statements, 0);
	free(statements);
}

void generator(FILE* file, ast_list_t* ast)
{
	fprintf(file, "#include <stdbool.h>\n\n");

	ast_list_t* func = ast;
	while (func != NULL)
	{
		function(file, func->value);
		fprintf(file, "\n\n");

		func = func->next;
	}
}
