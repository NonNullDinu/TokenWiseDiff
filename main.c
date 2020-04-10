#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "comparator.h"

struct in_file_t {
	size_t index;
	char path[256];
	struct tokens_container_t container;
	bool need, createdSuccessfully;
};

struct pair_of_in_files_t {
	size_t real_index1, real_index2;
};

struct in_file_t files[2000];
struct pair_of_in_files_t pairs[100000];

struct opts_t {
	int fancy_output, skip_counts, really_simple_output;
	char syntax_file[256];
	size_t in_file_count, pairs_count;
};

static struct opts_t process_args(int argc, char **argv);

int main(int argc, char **argv) {
	struct opts_t opts = process_args(argc, argv);
	struct syntax_t syntax = getSyntax(opts.syntax_file);
	for (size_t i = 0; i < opts.in_file_count; i++) {
		if (!files[i].need) { continue; }
		FILE *fd_wrap = fopen(files[i].path, "r");
		if (!fd_wrap) {
			files[i].createdSuccessfully = false;
			continue;
		}
		files[i].createdSuccessfully = read_parse(&files[i].container, fd_wrap, &syntax);
		fclose(fd_wrap);
	}
	for (size_t i = 0; i < opts.pairs_count; i++) {
		size_t i1 = pairs[i].real_index1, i2 = pairs[i].real_index2;
		if (!files[i1].createdSuccessfully || !files[i2].createdSuccessfully) { continue; }
		struct result_t res = compare(&files[i1].container, &files[i2].container, &syntax);
		if (opts.fancy_output) {
			printf("BEGIN\nBetween:%ld and %ld\n", files[i1].index,
			       files[i2].index);
			if (!opts.skip_counts) {
				printf("similarities:%ld\ntotal:%ld\n", res.matches, res.total);
			}
			printf("percentage:%Lf\nEND\n", ((long double) res.matches) / ((long double) res.total));
		} else if (opts.really_simple_output) {
			printf("(%ld,%ld)", files[i1].index,
			       files[i2].index);
			if (!opts.skip_counts) {
				printf(":%ld:%ld", res.matches, res.total);
			}
			printf(":%Lf\n", ((long double) res.matches) / ((long double) res.total));
		} else {
			printf("BEGIN\n%ld,%ld\n", files[i1].index,
			       files[i2].index);
			if (!opts.skip_counts) {
				printf("%ld\n%ld\n", res.matches, res.total);
			}
			printf("%Lf\nEND\n", ((long double) res.matches) / ((long double) res.total));
		}
	}
	return 0;
}

static struct opts_t process_args(int argc, char **argv) {
	struct opts_t opts;
	opts.fancy_output = opts.in_file_count = opts.skip_counts = opts.really_simple_output = 0;
	strcpy(opts.syntax_file, "");
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			size_t index;
			char *end_ptr;
			index = strtoul(argv[++i], &end_ptr, 10);
			files[opts.in_file_count] = (struct in_file_t) {.container = {}, .index = index};
			strcpy(files[opts.in_file_count].path, end_ptr + 1);
			files[opts.in_file_count].need = false;
			opts.in_file_count++;
		} else if (strcmp(argv[i], "-p") == 0) {
			size_t index1, index2;
			char *end_ptr;
			size_t real_i1 = -1, real_i2 = -1;
			index1 = strtoul(argv[++i], &end_ptr, 10);
			index2 = strtoul(end_ptr + 1, &end_ptr, 10);
			for (size_t j = 0; j < opts.in_file_count && (real_i1 == -1 || real_i2 == -1); j++) {
				if (files[j].index == index1) {
					real_i1 = j;
				}
				if (files[j].index == index2) {
					real_i2 = j;
				}
			}
			if (real_i1 == -1) {
				fprintf(stderr, "No file found for index %lu\n", index1);
				continue;
			}
			if (real_i2 == -1) {
				fprintf(stderr, "No file found for index %lu\n", index2);
				continue;
			}
			pairs[opts.pairs_count].real_index1 = real_i1;
			pairs[opts.pairs_count].real_index2 = real_i2;
			files[real_i1].need = files[real_i2].need = true;
			opts.pairs_count++;
		} else if (strcmp(argv[i], "-s") == 0) {
			strcpy(opts.syntax_file, argv[i + 1]);
			i++;
		} else if (strcmp(argv[i], "-f") == 0) {
			opts.fancy_output = 1;
		} else if (strcmp(argv[i], "--skip-counts") == 0) {
			opts.skip_counts = 1;
		} else if (strcmp(argv[i], "--really-simple-output") == 0) {
			opts.really_simple_output = 1;
		}
	}
	return opts;
}