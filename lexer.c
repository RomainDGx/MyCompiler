#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

char* lexer_getalphanum(buffer_t* buffer)
{
	// Char size is 1o => number max length = 1 * LEXEM_SIZE
	// '+ 1' is the reserved place for the '\0' character (end of string)
	char* string = malloc(LEXEM_SIZE + 1);
	assert(string != NULL);

	char* index = string;

	buf_lock(buffer);

	char c = buf_getchar_after_blank(buffer);

	// Check if start with alphabetic character
	if (!buf_eof_strict(buffer) && isalpha(c))
	{
		do
		{
			// TODO: Throws when lexel size is too long
			assert(index < string + LEXEM_SIZE);

			*index = c;
			index++;

			c = buf_getchar(buffer);
		}
		while (!buf_eof_strict(buffer) && isalnum(c));
	}

	assert(index < string + LEXEM_SIZE + 1);
	*index = '\0';

	buf_rollback_and_unlock(buffer, 1);

	return string;
}

char* lexer_getalphanum_rollback(buffer_t* buffer)
{
	char* string = malloc(LEXEM_SIZE + 1);
	assert(string != NULL);

	char* index = string;

	buf_lock(buffer);

	char c = buf_getchar_after_blank(buffer);

	if (!buf_eof_strict(buffer) && isalpha(c))
	{
		do
		{
			// TODO: Throws when lexel size is too long
			assert(index < string + LEXEM_SIZE);

			*index = c;
			index++;

			c = buf_getchar(buffer);
		}
		while (!buf_eof_strict(buffer) && isalnum(c));
	}

	assert(index < string + LEXEM_SIZE + 1);
	*index = '\0';

	buf_rollback_and_unlock(buffer, index - string);

	return string;
}

long lexer_getnumber(buffer_t* buffer)
{
	char* string = malloc(LEXEM_SIZE + 1);
	assert(string != NULL);

	char* index = string;

	buf_lock(buffer);

	char c = buf_getchar_after_blank(buffer);

	if (!buf_eof_strict(buffer))
	{
		if (c == '-')
		{
			*index = c;
			index++;
			c = buf_getchar(buffer);
		}


		while (!buf_eof_strict(buffer) && isdigit(c))
		{
			// TODO: Throws when lexel size is too long
			assert(index < string + LEXEM_SIZE);

			*index = c;
			index++;

			c = buf_getchar(buffer);
		}
	}

	assert(index < string + LEXEM_SIZE + 1);
	*index = '\0';

	buf_rollback_and_unlock(buffer, 1);

	long number = strtol(string, NULL, 10);

	free(string);

	return number;
}
