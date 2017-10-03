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

struct json_token
json_gettoken(FILE *stream)
{
	int	c;
	struct json_token	token	= { (enum json_type)0 };

	while ((c = getc(stream)) != EOF) {
		if (c == ':') {
			token.type = json_typeflag_pair;
		} else if (c != ',' && !isspace(c)) {
			break;
		}
	}

	switch (c) {
	case '"':
		switch (c = getc(stream)) {
		case '"':
			token.type |= json_typeflag_end;
			break;
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
			/* TODO: \u */
			}
		default:
			token.data.c = c;

			/* NB!
			 * the use of the term 'push back' in the POSIX standard in makes
			 * it unclear whether the character passed to ungetc must originate
			 * from the stream, so the following might not be POSIX conforming.
			 */
			ungetc('"', stream);
		}
		token.type |= json_type_string;
		break;
	case 'f':
		getc(stream); /* a */
		token.type |= json_type_false;
	case 't':
		token.type |= json_type_true;
	case 'n':
		getc(stream); /* l, r, u */
		getc(stream); /* s, u, l */
		getc(stream); /* e, e, l */
		token.type |= json_type_null;
		break;
	case ']':
		token.type = json_typeflag_end;
	case '[':
		token.type |= json_type_array;
		break;
	case '}':
		token.type = json_typeflag_end;
	case '{':
		token.type |= json_type_object;
		break;
	default:
		if (!isdigit(c)) break;
	case '-':
		ungetc(c, stream);
		fscanf(stream, JSON_NUMBER_FORMAT, &token.data.num);
		token.type |= json_type_number;
		break;
	}

	return token;
}
