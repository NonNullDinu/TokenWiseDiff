#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/comparator.h"

char file_1_loc[256];
char file_2_loc[256];

TOKENS_CONTAINER INTERNAL_1, INTERNAL_2;
typedef struct
{
    int fancy_output;
    char syntax_file[256];
} OPTS;

OPTS process_args(int argc, char **argv);
SYNTAX getSyntax(char *file);

int main(int argc, char **argv)
{
    OPTS opts = process_args(argc, argv);
    SYNTAX syntax = getSyntax(opts.syntax_file);
    FILE *f1 = fopen(file_1_loc, "r");
    FILE *f2 = fopen(file_2_loc, "r");
    read_parse(&INTERNAL_1, f1, &syntax);
    read_parse(&INTERNAL_2, f2, &syntax);
    fclose(f1);
    fclose(f2);
    RESULT res = compare(&INTERNAL_1, &INTERNAL_2, &syntax);
    if (opts.fancy_output)
        printf("Similarities in tokens: %ld\nTotal: %ld\n", res.matches, res.total);
    else
        printf("%ld\n%ld\n", res.matches, res.total);
    deallocate_syntax(&syntax);
    return 0;
}

OPTS process_args(int argc, char **argv)
{
    OPTS opts;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            if (file_1_loc[0] == '\0')
            {
                strcpy(file_1_loc, argv[i + 1]);
                i++;
            }
            else if (file_2_loc[0] == '\0')
            {
                strcpy(file_2_loc, argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "ERROR: found more than 2 input files\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            strcpy(opts.syntax_file, argv[i + 1]);
            i++;
        }
        else if(strcmp(argv[i], "-f") == 0)
            opts.fancy_output = 1;
    }
    if (file_2_loc[0] == '\0')
    {
        fprintf(stderr, "ERROR: found less than 2 input files\n");
        exit(2);
    }
    return opts;
}

SYNTAX getSyntax(char *file)
{
    SYNTAX s;
    FILE *f = fopen(file, "r");
    char input[131072];
    fread(input, 1, 131072, f);
    fclose(f);
    char *p = strtok(input, "\n");
    int section = 0;
    while (p != NULL)
    {
        if (strcmp(p, "LANG_TOKENS:") == 0)
            section = 1;
        else if (strcmp(p, "SAME:") == 0)
            section = 2;
        else if (strcmp(p, "END") == 0)
            break;
        else
        {
            if (p[0] == '#')
            {
                p = strtok(NULL, "\n");
                continue;
            }
            if (section == 1)
            {
                s.tokens[s.number_of_tokens].type = s.number_of_tokens;
                strcpy(s.tokens[s.number_of_tokens].blueprint, p);
                if(p[0] == '@'){
                    if(strcmp(p, "@std_identifier") == 0){
                        int reti = regcomp(&s.tokens[s.number_of_tokens].regex, "^[a-zA-Z_][a-zA-Z0-9_]*", 0);
                        if(reti){
                            fprintf(stderr, "Could not compile regular expression for %s\n", p);
                            exit(3);
                        }
                    }
                    else if(strcmp(p, "@std_number") == 0){
                        int reti = regcomp(&s.tokens[s.number_of_tokens].regex, "^(0[0-7]+|0x[0-9a-fA-F]+|[0-9]+)", REG_EXTENDED);
                        if(reti){
                            fprintf(stderr, "Could not compile regular expression for %s\n", p);
                            exit(3);
                        }
                    }
                }
                s.number_of_tokens++;
            }
            else if (section == 2)
            {
            }
        }
        p = strtok(NULL, "\n");
    }
    return s;
}