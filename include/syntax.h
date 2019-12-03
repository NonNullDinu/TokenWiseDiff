#ifndef SYNTAX_H
#define SYNTAX_H

#include "tokens.h"

typedef struct {
    int number_of_tokens;
    B_TOKEN tokens[2048];
} SYNTAX;

void deallocate_syntax(SYNTAX*);

#endif