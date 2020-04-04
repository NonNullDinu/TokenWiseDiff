#ifndef SYNTAX_H
#define SYNTAX_H

#include "tokens.h"

struct syntax_t {
	int number_of_tokens;
	struct btoken_t tokens[2048];
	unsigned has_c_cpp_comments;
	unsigned has_std_strings;
};

struct syntax_t getSyntax(const char* file);

#endif