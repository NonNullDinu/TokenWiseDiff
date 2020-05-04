#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comparator.h"
#include "syntax.h"
#include "comparing_algorithm.h"

//int min(int a, int b) {
//	return a < b ? a : b;
//}
static int special_matches(struct btoken_t blueprint, const char *txt);

static void token_cpy(struct token_t *adr, const struct btoken_t t, char *p, int l) {
	adr->type = t.type;
//	strncpy(adr->data, p, min(l, 255));
//	adr->data[min(l, 255)] = '\0';
}

static int matches(struct btoken_t t, char *txt) {
	if (t.blueprint[0] == '@')
		return special_matches(t, txt);
	else
		return strncmp(t.blueprint, txt, strlen(t.blueprint)) == 0;
}

static int special_matches(struct btoken_t blueprint, const char *txt) {
	if (strcmp(blueprint.blueprint, "@std_identifier") == 0) {
		return (txt[0] >= 'A' && txt[0] <= 'Z') || (txt[0] >= 'a' && txt[0] <= 'z') || txt[0] == '_';
	} else if (strcmp(blueprint.blueprint, "@std_number") == 0) {
		return (txt[0] >= '0' && txt[0] <= '9');
	} else if (strcmp(blueprint.blueprint, "@std_string") == 0) {
		return txt[0] == '"';
	} else if (strcmp(blueprint.blueprint, "@std_char") == 0) {
		return txt[0] == '\'';
	} else { return 0; }
}

static ssize_t token_len(struct btoken_t t) {
	if (t.blueprint[0] == '@')
		return -1;
	else
		return strlen(t.blueprint);
}

static size_t removeComments(char *program, unsigned has_std_strings) {
	size_t n = strlen(program);
	unsigned s_cmt = 0;
	unsigned m_cmt = 0;
	size_t k = 0;
	unsigned in_string = 0;
	unsigned escaped = 0;

	for (int i = 0; i < n; i++) {
		if (has_std_strings && program[i] == '"' && !m_cmt && !s_cmt && !in_string) {
			in_string = 1;
			program[k++] = program[i];
		} else if (has_std_strings && in_string && program[i] == '"' && !escaped) {
			in_string = 0;
			program[k++] = program[i];
		} else if (has_std_strings && in_string) {
			if (program[i] == '\\')
				escaped = !escaped;
			else
				escaped = 0;
			program[k++] = program[i];
		} else if (s_cmt == 1 && program[i] == '\n') {
			s_cmt = 0;
		} else if (m_cmt == 1 && program[i] == '*' && program[i + 1] == '/' && !in_string)
			m_cmt = 0, i++;
		else if (s_cmt || m_cmt)
			continue;
		else if (program[i] == '/' && program[i + 1] == '/' && !in_string)
			s_cmt = 1, i++;
		else if (program[i] == '/' && program[i + 1] == '*' && !in_string)
			m_cmt = 1, i++;
		else
			program[k++] = program[i];
	}
	program[k] = 0;
	return k;
}

bool read_parse(struct tokens_container_t *container, FILE *file, struct syntax_t *s) {
	char *input_arr = malloc(131072);
	if (!input_arr) { return false; }
	size_t in_size = fread(input_arr, 1, 131072, file);
	input_arr[in_size] = '\0';
	if (s->has_c_cpp_comments)
		in_size = removeComments(input_arr, s->has_std_strings);
	size_t ptr = 0;
	int token_ptr = 0;
	while (ptr < in_size) {
		while (input_arr[ptr] == ' ' || input_arr[ptr] == '\n' || input_arr[ptr] == '\r' || input_arr[ptr] == '\t' ||
		       input_arr[ptr] == '#') {
			if (input_arr[ptr] == ' ' || input_arr[ptr] == '\n' || input_arr[ptr] == '\r' || input_arr[ptr] == '\t')
				ptr++;
			else if (input_arr[ptr] == '#')
				ptr = strchr(input_arr + ptr, '\n') - input_arr + 1;
		}
		if (input_arr[ptr] == '\0')
			break;
		unsigned token_found = 0;
		for (int i = 0; i < s->number_of_tokens; i++) {
			if (matches(s->tokens[i], input_arr + ptr) == 1) {
				token_found = 1;
				ssize_t l = token_len(s->tokens[i]);
				int p1 = ptr;
				if (l == -1) {
					if (strcmp(s->tokens[i].blueprint, "@std_identifier") == 0) {
						ptr++;
						while ((input_arr[ptr] >= 'a' && input_arr[ptr] <= 'z') ||
						       (input_arr[ptr] >= 'A' && input_arr[ptr] <= 'Z') ||
						       (input_arr[ptr] >= '0' && input_arr[ptr] <= '9') || input_arr[ptr] == '_')
							ptr++;
					} else if (strcmp(s->tokens[i].blueprint, "@std_number") == 0) {
						if (input_arr[ptr] == '0') {
							ptr++;
							if (input_arr[ptr] == 'x') {
								ptr++;
								while ((input_arr[ptr] >= '0' && input_arr[ptr] <= '9') ||
								       (input_arr[ptr] >= 'A' && input_arr[ptr] <= 'F') ||
								       (input_arr[ptr] >= 'a' && input_arr[ptr] <= 'f'))
									ptr++;
							} else {
								while (input_arr[ptr] >= '0' && input_arr[ptr] <= '7')
									ptr++;
							}
						} else
							while (input_arr[ptr] >= '0' && input_arr[ptr] <= '9')
								ptr++;
					} else if (strcmp(s->tokens[i].blueprint, "@std_string") == 0) {
						unsigned escaped = 0;
						for (ptr++; ptr < in_size; ptr++) {
							if (input_arr[ptr] == '"' && !escaped) {
								break;
							} else {
								if (input_arr[ptr] == '\\')
									escaped = !escaped;
								else
									escaped = 0;
							}
						}
						ptr++;
					} else if (strcmp(s->tokens[i].blueprint, "@std_char") == 0) {
						unsigned escaped = 0;
						for (ptr++; ptr < in_size; ptr++) {
							if (input_arr[ptr] == '\'' && !escaped) {
								break;
							} else {
								if (input_arr[ptr] == '\\')
									escaped = !escaped;
								else {
									escaped = 0;
								}
							}
						}
						ptr++;
					}
					l = ptr - p1;
				} else
					ptr += l;
				token_cpy(&(container->tokens[token_ptr++]), s->tokens[i], input_arr + p1, l);
				break;
			}
		}
		if (token_found == 0) {
			fprintf(stderr, "Cannot parse token at:\n%s\nFirst char: %d", input_arr + ptr, input_arr[ptr]);
			free(input_arr);
			return false;
		}
	}
	container->token_count = token_ptr;
	free(input_arr);
	return true;
}

struct result_t compare(struct tokens_container_t *c1, struct tokens_container_t *c2, struct syntax_t *s) {
	struct result_t result = (struct result_t) {.matches = 0, .total = c1->token_count + c2->token_count};
	LCS(c1->tokens, c2->tokens, c1->token_count, c2->token_count, &result.matches);
	return result;
}