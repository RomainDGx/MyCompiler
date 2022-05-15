#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

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
	exit(exit_code);
}
