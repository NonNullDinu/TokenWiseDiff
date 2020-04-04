#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <stdio.h>
#include <stdbool.h>
#include "tokens.h"
#include "syntax.h"

struct tokens_container_t {
	struct token_t tokens[65536];
	size_t token_count;
};

struct result_t {
	size_t matches, total;
};

bool read_parse(struct tokens_container_t *, FILE *, struct syntax_t *);

struct result_t compare(struct tokens_container_t *, struct tokens_container_t *, struct syntax_t *);

#endif