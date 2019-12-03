#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "../include/comparator.h"
#include "../include/syntax.h"
#include "../include/comparing_algorithm.h"

void token_cpy(TOKEN *adr, const B_TOKEN t, char *p)
{
    adr->type = t.type;
    adr->data = p;
}

int matches(B_TOKEN t, char *txt)
{
    if (t.blueprint[0] == '@')
    {
        return regexec(&t.regex, txt, 0, NULL, 0) == 0;
    }
    else
        return strncmp(t.blueprint, txt, strlen(t.blueprint)) == 0;
}

int token_len(B_TOKEN t)
{
    if (t.blueprint[0] == '@')
        return -1;
    else
        return strlen(t.blueprint);
}

void read_parse(TOKENS_CONTAINER *container, FILE *file, SYNTAX *s)
{
    char *input = (char *)malloc(131072);
    size_t in_size = fread(input, 1, 131072, file);
    int ptr = 0;
    int token_ptr = 0;
    while (ptr < in_size)
    {
        while (input[ptr] == ' ' || input[ptr] == '\n')
            ptr++;
        for (int i = 0; i < s->number_of_tokens; i++)
        {
            if (matches(s->tokens[i], input + ptr) == 1)
            {
                token_cpy(&(container->tokens[token_ptr++]), s->tokens[i], input + ptr);
                int l = token_len(s->tokens[i]);
                if (l == -1)
                {
                    if(strcmp(s->tokens[i].blueprint, "@std_identifier") == 0){
                        ptr++;
                        while((input[ptr] >= 'a' && input[ptr] <= 'z') || (input[ptr] >= 'A' && input[ptr] <= 'Z') || (input[ptr] >= '0' && input[ptr] <= '9') || input[ptr] == '_')
                            ptr++;
                    }
                }
                else
                    ptr += l;
                break;
            }
        }
    }
    container->token_count = token_ptr;
}

int compare(TOKENS_CONTAINER *c1, TOKENS_CONTAINER *c2, SYNTAX *s)
{
    int *m1 = 
    return 0;
}