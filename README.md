# json-stdio
Lightweight JSON lexer using C stdio streams

## Features
-	simple
	-	ANSI C
	-	2 files ([1 source](json-stdio.c) + [1 header](json-stdio.h))
	-	~120 lines of code
	-	stdio-like interface
	-	single function
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

struct json_token json_gettoken(FILE *stream);
```

## DESCRIPTION
The `"json-stdio.h"` header defines the `JSON_NUMBER_TYPE` and
`JSON_NUMBER_FORMAT` macros if they are undefined.

The `"json-stdio.h"` header defines the `json_number` type through typedef as
the expansion of the `JSON_NUMBER_TYPE` macro.

The `"json-stdio.h"` header defines the `json_type` enumeration with the
following members:
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
json_typeflag_end  Indicates that the token is a closing token (of a string,
                   array or object value).
```

The `"json-stdio.h"` header defines the `json_token` structure with the
following members:
```C
enum json_type type     The type of the token
json_number    data.num The value of a JSON number value token
char           data.c   The next character of a JSON string value token
```

json-stdio requires no explicit initialization. To start using it, simply pass
your stdio stream to `json_gettoken()`.

The `json_gettoken()` function attempts to get the next token in `stream`.

## RETURN VALUE
The `json_gettoken()` function returns a **json_token** structure with the
`type` member set to an **enum json_type** enumeral representing the type of the
next token, or `json_type_eof` if the stream is at end-of-file or a read error
occurs. If the token is a JSON number value token, it is converted to a
**json_number** and stored in the `data.num` member. If the token is a JSON
string value token, the next character of the string is stored in the `data.c`
member. Any JSON character escape sequence is converted.
