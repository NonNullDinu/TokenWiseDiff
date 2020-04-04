#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "comparator.h"

struct in_file_t {
	size_t index;
	char path[256];
	struct tokens_container_t container;
	bool createdSuccessfully;
};

struct in_file_t files[3];

struct opts_t {
	int fancy_output;
	char syntax_file[256];
	size_t in_file_count;
};

static struct opts_t process_args(int argc, char **argv);

int main(int argc, char **argv) {
	struct opts_t opts = process_args(argc, argv);
	struct syntax_t syntax = getSyntax(opts.syntax_file);
	for (size_t i = 0; i < opts.in_file_count; i++) {
		FILE *fd_wrap = fopen(files[i].path, "r");
		if (!fd_wrap) {
			files[i].createdSuccessfully = false;
			continue;
		}
		files[i].createdSuccessfully = read_parse(&files[i].container, fd_wrap, &syntax);
		fclose(fd_wrap);
	}
	for (size_t i = 1; i < opts.in_file_count; ++i) {
		if (!files[i].createdSuccessfully)continue;
		for (size_t j = 0; j < i; ++j) {
			if (!files[j].createdSuccessfully) continue;
			struct result_t res = compare(&files[i].container, &files[j].container, &syntax);
			if (opts.fancy_output)
				printf("BEGIN\nBetween:%ld and %ld\nsimilarities:%ld\ntotal:%ld\npercentage:%Lf\nEND\n", files[j].index,
				       files[i].index, res.matches, res.total,
				       ((long double) res.matches) / ((long double) res.total));
			else
				printf("BEGIN\n%ld,%ld\n%ld\n%ld\n%Lf\nEND\n", files[j].index, files[i].index, res.matches, res.total,
				       ((long double) res.matches) / ((long double) res.total));
		}
	}
	return 0;
}

static struct opts_t process_args(int argc, char **argv) {
	struct opts_t opts;
	opts.fancy_output = opts.in_file_count = 0;
	strcpy(opts.syntax_file, "");
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			size_t index;
			++i;
			char *end_ptr;
			index = strtoul(argv[i], &end_ptr, 10);
			fflush(stdout);
			files[opts.in_file_count] = (struct in_file_t) {.container = {}, .index = index};
			strcpy(files[opts.in_file_count].path, end_ptr + 1);
			opts.in_file_count++;
		} else if (strcmp(argv[i], "-s") == 0) {
			strcpy(opts.syntax_file, argv[i + 1]);
			i++;
		} else if (strcmp(argv[i], "-f") == 0)
			opts.fancy_output = 1;
	}
	return opts;
}