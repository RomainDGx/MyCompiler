#include <stdio.h>

#include "ast.h"
#include "buffer.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"

int main(int argc, char* argv[])
{
	char* file_name = argv[1];

	FILE* file = fopen(file_name, "r");

	if (file == NULL)
	{
		printf("Invalid file path.");
		return -1;
	}

	buffer_t buffer;

	buf_init(&buffer, file);

	//char* string = lexer_getalphanum(&buffer);
	//printf("\"%s\"", string);

	//char* string = lexer_getalphanum_rollback(&buffer);
	//printf("\"%s\"", string);

	//long number = lexer_getnumber(&buffer);
	//printf("\"%ld\"", number);

	symbol_t* table = NULL;
	ast_list_t* functions = parser(&buffer, &table);


	fclose(file);

	return 0;
}
