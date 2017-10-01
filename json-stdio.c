/*
 * json-stdio - lightweight JSON lexer using C stdio streams
 * Copyright (c) 2017 Oskar Sveinsen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ctype.h>
#include <stdio.h>

#include "json-stdio.h"

/* TODO: handle errors and malformed input
 * TODO: handle locales
 * TODO: prettify
 */

/* works with any POSIX compliant character set
 * NB! evaluates c more than once
 */
#define	JSON_HEXDIGIT_DECODE_(c)	(isdigit(c) ? c - '0' : tolower(c) - 'a' + 0xa)

enum json_type
json_gettype(FILE *stream)
{
	int	c;
	enum json_type	type	= (enum json_type)0;

	while ((c = getc(stream)) != EOF) {
		if (c == ':') {
			type = json_typeflag_pair;
		} else if (c != ',' && !isspace(c)) {
			break;
		}
	}

	switch (c) {
	default:
		if (!isdigit(c)) break;
	case '-':
		ungetc(c, stream);
		return type | json_type_number;
	case '"':
		return type | json_type_string;
	case 'f':
		getc(stream); /* a */
		type |= json_type_false;
	case 't':
		type |= json_type_true;
	case 'n':
		getc(stream); /* l, r, u */
		getc(stream); /* s, u, l */
		getc(stream); /* e, e, l */
		return type | json_type_null;
	case ']':
		type = json_typeflag_end;
	case '[':
		return type | json_type_array;
	case '}':
		type = json_typeflag_end;
	case '{':
		return type | json_type_object;
	}

	return json_type_eof;
}

json_number
json_getnumber(FILE *stream)
{
	json_number	num	= 0;

	fscanf(stream, JSON_NUMBER_FORMAT, &num);
	return num;
}

int
json_getc(FILE *stream)
{
	int	c;

	switch (c = getc(stream)) {
	case EOF:
	case '"':
		return EOF;
	case '\\':
		switch (c = getc(stream)) {
			int	d, e;

		case 'b':
			c = '\b';
			break;
		case 'f':
			c = '\f';
			break;
		case 'n':
			c = '\n';
			break;
		case 'r':
			c = '\r';
			break;
		case 't':
			c = '\t';
			break;
		case 'u':
			d = getc(stream);
			e = getc(stream);
			c = (JSON_HEXDIGIT_DECODE_(d) << 4)
				| JSON_HEXDIGIT_DECODE_(e);

			/* NB!
			 * the use of the term 'push back' in the POSIX standard in makes
			 * it unclear whether the character passed to ungetc must originate
			 * from the stream
			 */
			d = getc(stream);
			e = getc(stream);
			ungetc((JSON_HEXDIGIT_DECODE_(d) << 4)
				| JSON_HEXDIGIT_DECODE_(e), stream);

			break;
		}
		break;
	}

	return c;
}

char
*json_gets(char *s, int n, FILE *stream)
{
	int	i;

	for (i = 0; i < n; i++) {
		int	c = json_getc(stream);

		if (c == EOF) break;
		s[i] = c;
	}
	s[i] = '\0';
	return s;
}
