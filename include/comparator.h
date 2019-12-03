#ifndef COMPARATOR_H
#define COMPARATOR_H
#include <stdio.h>
#include "tokens.h"
#include "syntax.h"
typedef struct {
    TOKEN tokens[65536];
    int token_count;
} TOKENS_CONTAINER;

void read_parse(TOKENS_CONTAINER*, FILE*, SYNTAX*);

int compare(TOKENS_CONTAINER*, TOKENS_CONTAINER*, SYNTAX*);
#endif