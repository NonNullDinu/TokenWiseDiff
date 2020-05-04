#ifndef COMP_ALGO_H
#define COMP_ALGO_H

#include "tokens.h"
#include <stddef.h>

void LCS(
		const struct token_t *items1, const struct token_t *items2,
		unsigned count1, unsigned count2, size_t *matches_count);

#endif