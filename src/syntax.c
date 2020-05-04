//
// Created by dinu on 4/4/20.
//

#include "syntax.h"
#include <stdio.h>
#include <string.h>

struct syntax_t getSyntax(const char *file) {
	struct syntax_t s;
	s.has_c_cpp_comments = s.has_std_strings = 0;
	FILE *f = fopen(file, "r");
	if (!f) { perror("Cannot open syntax"); }
	char input[131072];
	fread(input, 1, 131072, f);
	fclose(f);
	char *p = strtok(input, "\n");
	int section = 0;
	while (p != NULL) {
		if (strcmp(p, "LANG_TOKENS:") == 0)
			section = 1;
		else if (strcmp(p, "CONFIG:") == 0)
			section = 2;
		else if (strcmp(p, "END") == 0)
			break;
		else {
			if (p[0] == '#') {
				p = strtok(NULL, "\n");
				continue;
			}
			if (section == 1) {
				s.tokens[s.number_of_tokens].type = s.number_of_tokens;
				strcpy(s.tokens[s.number_of_tokens].blueprint, p);
				s.number_of_tokens++;
			} else if (section == 2) {
				if (strcmp(p, "HAS_STD_COMMENTS") == 0)
					s.has_c_cpp_comments = 1;
				if (strcmp(p, "HAS_STD_STRINGS") == 0)
					s.has_std_strings = 1;
			}
		}
		p = strtok(NULL, "\n");
	}
	return s;
}