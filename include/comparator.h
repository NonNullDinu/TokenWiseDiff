#ifndef COMPARATOR_H
#define COMPARATOR_H
#include <stdio.h>
#include "tokens.h"
#include "syntax.h"
typedef struct
{
    TOKEN tokens[65536];
    size_t token_count;
} TOKENS_CONTAINER;

typedef struct
{
    size_t matches, total;
} RESULT;

void read_parse(TOKENS_CONTAINER *, FILE *, SYNTAX *);

RESULT compare(TOKENS_CONTAINER *, TOKENS_CONTAINER *, SYNTAX *);
#endif