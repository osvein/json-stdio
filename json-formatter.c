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

#include <stdio.h>

#include "json.h"

unsigned int	indent;
int	spaces	= 1;

void
newline(void)
{
	int	i;

	if (!spaces) return;
	putchar('\n');
	for (i = 0; i < indent; i++) putchar('\t');
}

int
main(int argc, char *argv[])
{
	struct json_token	token;
	int	start	= 1,	firstval	= 1;

	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') spaces = 0;

	while ((token = json_gettoken(stdin)).type) {
		char	bracketoff	= 0;

		if (token.type & json_typeflag_pair) {
			putchar(':');
			if (spaces) putchar(' ');
		} else if (!(token.type & json_typeflag_end)) {
			if (!start) putchar(',');
			if (!firstval) newline();
		}

		start = firstval = 0;

		switch (token.type & json_typemask) {
			int	c;

		case json_type_null:
			puts("null");
			break;
		case json_type_false:
			puts("false");
			break;
		case json_type_true:
			puts("true");
			break;
		case json_type_number:
			printf(JSON_NUMBER_FORMAT, token.data.num);
			break;
		case json_type_string:
			putchar('"');
			do {
				putchar(token.data.c);
			} while (!((token = json_gettoken(stdin)).type & json_typeflag_end));
			putchar('"');
			break;
		case json_type_object:
			bracketoff = 0x20;
		case json_type_array:
			if (token.type & json_typeflag_end) {
				bracketoff += 2;
				indent--;
				newline();
			} else {
				start = 1;
				indent++;
			}
			putchar('[' + bracketoff);
			break;
		}
	}

	if (ferror(stdin) || ferror(stdout)) {
		perror(*argv);
		return 1;
	}

	putchar('\n');
	return 0;
}
