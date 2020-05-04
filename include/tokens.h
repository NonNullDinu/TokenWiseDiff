#ifndef TOKENS_H
#define TOKENS_H

struct token_t {
	int type;
};

struct btoken_t {
	int type;
	char blueprint[256];
};
#endif