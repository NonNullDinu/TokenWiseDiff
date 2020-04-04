# Syntax file
The syntax file has 2 sections
## The syntax config
This section starts with the line:
```
CONFIG:
```
And contains values that tells the parser about the language.
- `HAS_STD_COMMENTS`. If this is present in the config, it means the described language supports the `/*...*/` and `//...` comments
- `HAS_STD_STRINGS`. If this is present in the config, it means the described language supports constant strings with characters that can be escaped<br/>
Examples include:
```c
"abc"
"de\nf"
"gh\t \\ \""
```
## The language tokens
This section starts with the line:
```
LANG_TOKENS:
```
And then all the symbols, operators, keywords of the language, and optionally the following:
```
@std_identifier
@std_number
@std_string
@std_char
```
The `@std_identifier` tells the parser that it can expect standard C identifiers(e.g. matches this regex `/^[a-zA-Z_][a-zA-Z0-9_]*$/`)

The `@std_number` tells the parser that it can expect numbers written in octal(have `0` at the front), hexadecimal(have `0x`), and decimal notation.

The `@std_string` tells the parser that it can expect C strings, as described above. This should be used together with `HAS_STD_STRINGS` in the config.

The `@std_char` tells the parser that it can expect C characters, like `'a'`, `'b'`, `'\''`, `'"'`

**In the end, it should end with a line that only contains:**
```
END
```
