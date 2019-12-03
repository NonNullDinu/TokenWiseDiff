#include "../include/syntax.h"
void deallocate_syntax(SYNTAX* s){
    for(int i = 0; i < s->number_of_tokens; i++){
        if(s->tokens[i].blueprint[0] == '@'){
            regfree(&s->tokens[i].regex);
        }
    }
}