#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "ast.h"

char* copy_name(char* name)
{
	size_t len = strlen(name) + 1;
	char* out = malloc(sizeof(char) * len);
	assert(out != NULL);
	strncpy(out, name, len);
	return out;
}

void print_backtrace()
{
	/* not available on Win */
}

void parse_error(char* message, buffer_t* buffer, int exit_code)
{
	printf("%s\n", message);
	buf_print(buffer);
	fclose(buffer->fd);
	exit(exit_code);
}

type_e parse_type(char* name, buffer_t* buffer)
{
	if (strcmp(name, "entier") == 0)
	{
		return TYPE_INTEGER;
	}
	if (strcmp(name, "booleen") == 0)
	{
		return TYPE_BOOLEAN;
	}
	if (strcmp(name, "rien") == 0)
	{
		return TYPE_VOID;
	}
	parse_error("Invalid type", buffer, 1);
}