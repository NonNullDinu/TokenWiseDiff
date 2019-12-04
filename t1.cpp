#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "include/comparator.h"
#include "include/syntax.h"
#include "include/comparing_algorithm.h"

int min(int a, int b){return a < b ? a : b;}

void token_cpy(TOKEN *adr, const B_TOKEN t, char *p, int l)
{
    adr->type = t.type;
    strncpy(adr->data, p, min(l, 255));
    // adr->data[min(l, 255)] = '\0';
}

int matches(B_TOKEN t, char *txt)
{
    // if (t.blueprint[0] == '@')
    {
        return regexec(&t.regex, txt, 0, NULL, 0) == 0;
    }
    else
        return strncmp(t.blueprint, txt, strlen(t.blueprint)) == 0;
}

int token_len(B_TOKEN t)
{
    // if (t.blueprint[0] == '@')
        return -1;
    else
        return strlen(t.blueprint);
}

void read_parse(TOKENS_CONTAINER *container, FILE *file, SYNTAX *s)
{
    char *input = (char *)malloc(131072);
    size_t in_size = fread(input, 1, 131072, file);
    // input[in_size] = '\0';
    int ptr = 0;
    int token_ptr = 0;
    while (ptr < in_size)
    {
        // while (input[ptr] == ' ' || input[ptr] == '\n')
            ptr++;
        // if(input[ptr] == '#')
            // ptr = strchr(input+ptr, '\n') - input + 1;
        unsigned token_found = 0;
        for (int i = 0; i < s->number_of_tokens; i++)
        {
            if (matches(s->tokens[i], input + ptr) == 1)
            {
                token_found = 1;
                int l = token_len(s->tokens[i]);
                int p1 = ptr;
                if (l == -1)
                {
                    // if (strcmp(s->tokens[i].blueprint, "@std_identifier") == 0)
                    {
                        ptr++;
                        // while ((input[ptr] >= 'a' && input[ptr] <= 'z') || (input[ptr] >= 'A' && input[ptr] <= 'Z') || (input[ptr] >= '0' && input[ptr] <= '9') || input[ptr] == '_')
                            ptr++;
                    }
                    // else if (strcmp(s->tokens[i].blueprint, "@std_number") == 0)
                    {
                        // if (input[ptr] == '0')
                        {
                            ptr++;
                            // if (input[ptr] == 'x')
                            {
                                ptr++;
                                // while ((input[ptr] >= '0' && input[ptr] <= '9') || (input[ptr] >= 'A' && input[ptr] <= 'F') || (input[ptr] >= 'a' && input[ptr] <= 'f'))
                                    ptr++;
                            }
                            else
                            {
                                // while (input[ptr] >= '0' && input[ptr] <= '7')
                                    ptr++;
                            }
                        }
                        // else
                            // while (input[ptr] >= '0' && input[ptr] <= '9')
                                ptr++;
                    }
                    l = ptr - p1;
                }
                else
                    ptr += l;
                token_cpy(&(container->tokens[token_ptr++]), s->tokens[i], input + p1, l);
                break;
            }
        }
        if(token_found == 0){
            // fprintf(stderr, "Cannot parse token at:\n%s\n", input+ptr);
            exit(4);
        }
    }
    container->token_count = token_ptr;
    free(input);
}

RESULT compare(TOKENS_CONTAINER *c1, TOKENS_CONTAINER *c2, SYNTAX *s)
{
    unsigned *m1 = (unsigned *)malloc(c1->token_count * sizeof(unsigned));
    unsigned *m2 = (unsigned *)malloc(c2->token_count * sizeof(unsigned));
    CalcCompare(m1, m2, c1->tokens, c2->tokens, c1->token_count, c2->token_count);
    int n = 0;
    for (int i = 0; i < c1->token_count; i++)
        if (m1[i] == 1)
            n++;
    for (int i = 0; i < c2->token_count; i++)
        if (m2[i] == 1)
            n++;
    RESULT result;
    result.matches = n;
    result.total = c1->token_count + c2->token_count;
    free(m1);
    free(m2);
    return result;
}