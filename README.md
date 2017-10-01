# json-stdio
Lightweight JSON lexer using C stdio streams

## Features
-	simple
	-	ANSI C
	-	2 files ([1 source](json-stdio.c) + [1 header](json-stdio.h))
	-	~150 lines of code
	-	stdio-like interface
	-	4 functions (3 primitive)
	-	doesn't require initialization
-	operates on unbuffered and unseekable streams of JSON data
-	uses stdio streams for abstraction of
	-	memory (buffers, shared/typed memory objects)
	-	standard input
	-	files
	-	POSIX sockets
-	converts JSON numbers and strings to C data types
	-	configurable number precision
	-	handles escape sequences
	-	works with multibyte characters
-	no dynamic heap allocation
-	non-buffering
-	stateless
-	[permissive license](LICENSE) (MIT)

## SYNOPSIS
```C
#include <stdio.h>
#include "json-stdio.h"

enum json_type json_gettype(FILE *stream);
json_number    json_getnumber(FILE *stream);
char           json_getc(FILE *stream);
char          *json_gets(char *s, int n, FILE *stream);
```

## DESCRIPTION
The `"json-stdio.h"` header defines the `JSON_NUMBER_TYPE` and
`JSON_NUMBER_FORMAT` macros if they are undefined.

The `"json-stdio.h"` header defines the `json_number` type through typedef as
the expansion of the `JSON_NUMBER_TYPE` macro.

The `"json-stdio.h"` header defines the `json_type` enumeration with the
following members
```C
json_type_eof      Indicates an end-of-file condition.
json_type_null     Indicates a JSON null value token.
json_type_true     Indicates a JSON true value token.
json_type_false    Indicates a JSON true value token.
json_type_number   Indicates a JSON number value token.
json_type_string   Indicates a JSON string value token.
json_type_array    Indicates a JSON array value opening token.
json_type_object   Indicates a JSON object value opening token.
json_typemask      Mask for excluding the typeflags.
json_typeflag_pair Indicates that the token is the right-hand side of a JSON
                   object member (and consequently that the previous token was
                   the left-hand side).
json_typeflag_end  Indicates that the token is a closing token rather than an.
                   opening token.
```

json-stdio requires no explicit initialization. To start using it, simply pass
your stdio stream to `json_gettype()`.

The `json_gettype()` function attempts to get the type of the next token in
`stream`.

The `json_getnumber()` function obtains the JSON number in `stream`, converted
to a C floating-point number. It must be called if and only if the last call to
`json_gettype()` indicated that the next token is a number, before another call
to `json_gettype()` on the stream can be made.

The `json_getc()` function obtains the next byte of the JSON string in `stream`,
converted to an **int**. Since it operates on bytes, reading a multi-byte
character, or an escape sequence representing a multi-byte character, will
require multiple calls to `json_getc()`.

The `json_gets()` function reads bytes from the JSON string in `stream` into the
array pointed to by `s` until `n-1` bytes are read, or it reaches the end of the
string. A null byte will be written immediately after the last byte read into
the array.

The `json_getc()` or the `json_gets()` function must be called if and only if
the last call to `json_gettype()` on `stream` indicated that the next token is
a string, until the end of the string is reached, before another call to
`json_gettype()` on the stream can be made.

The `json_getc()` and `json_gets()` functions will convert any JSON character
escape sequences.

## RETURN VALUE
The `json_gettype()` function returns a **json_type** enumeral representing the
type of the next token, or `json_type_eof` if the stream is at end-of-file or a
read error occurs.

The `json_getnumber()` function returns the JSON number in the stream, converted
to a **json_number**.

The `json_getc()` function returns the next byte of the JSON string in the
stream, converted to an **int**, or EOF if the end of the string was reached.

The `json_gets()` function returns its `s` argument.
