#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "buffer.h"
#include "generator.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"

int main(int argc, char* argv[])
{
	char* file_name = argv[1];
	FILE* file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Invalid file path '%s'.", file_name);
		return -1;
	}

	buffer_t buffer;

	buf_init(&buffer, file);

	symbol_t* global_table = NULL;
	ast_list_t* functions = parser(&buffer, &global_table);

	fclose(file);

	file_name = argv[2];
	file = fopen(file_name, "w");
	if (file == NULL)
	{
		printf("Invalid file path '%s'.", file_name);
		return -1;
	}

	generator(file, functions);

	fclose(file);

	return 0;
}
