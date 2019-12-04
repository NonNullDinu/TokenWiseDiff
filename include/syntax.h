#ifndef SYNTAX_H
#define SYNTAX_H

#include "tokens.h"

typedef struct {
    int number_of_tokens;
    B_TOKEN tokens[2048];
    unsigned has_c_cpp_comments;
    unsigned has_std_strings;
} SYNTAX;

void deallocate_syntax(SYNTAX*);

#endif