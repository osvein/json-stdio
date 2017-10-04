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

#ifndef	JSON_NUMBER_TYPE
#define	JSON_NUMBER_TYPE	float
#endif
#ifndef	JSON_NUMBER_FORMAT
#define	JSON_NUMBER_FORMAT	"%f"
#endif

typedef	JSON_NUMBER_TYPE	json_number;

struct json_token {
	/* changing the values will mess up the case fallthroughs */
	enum json_type {
		json_type_eof	= 0,
		json_type_null	= 1,
		json_type_true	= 3, /* (json_type_null << 1) | 1; */
		json_type_false	= 7, /* (json_type_true << 1) | 1; */
		json_type_number	= 2,
		json_type_string	= 4,
		json_type_array	= 5,
		json_type_object	= 6,
		json_typemask	= 7,

		/* the token is part of the right-hand side of an object member */
		json_typeflag_pair	= 010,

		/* the token is the end of a value (array and object only) */
		json_typeflag_end	= 020
	} type;
	union {
		char c;
		json_number num;
	} data;
}	json_gettoken(FILE *);
