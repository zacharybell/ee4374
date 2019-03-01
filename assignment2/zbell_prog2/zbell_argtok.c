#include "zbell_argtok.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/*
 * Creates a copy of a subset of tokens from the token array.
 *
 * Args:
 *      args - the token array
 *      start - the beginning of the subset (must be within bounds in args)
 *      end - the end of the subset (must be within bounds and greater or equal to start
 *
 * Return:
 *      A copy of a subset of tokens
 */
char** sub_cpy(char** args, int start, int end) {
    char** cpy = (char**)calloc((end-start), sizeof(char*));

    int i;
    int j = start;
    for(i = 0; j < end; i++, j++) {
        cpy[i] = (char*)calloc((strlen(args[j])+1), sizeof(char));
        strcpy(cpy[i], args[j]);
    }

    return cpy;
}

/* 
 * Computes the number of tokens in a string.
 */
int n_tok(char* str) {

    bool tok_flag = false;
    int count = 0;
    char* p;    

    for (p=str; *p; p++) {
        if (*p == DELIMITER && tok_flag) {
            count++;
            tok_flag = false;
        }
        else if (*p != DELIMITER) tok_flag = true;
    }

    if (tok_flag) count++;
    
    return count;
}


/*
 * Computes the length of the next token in a string.
 *
 * NOTE: tok_len will return 0 if a delimiter prefixes the token.
 */
int tok_len(char* str) {
    
    int length = 0;
    char* p;
    
    for (p=str; *p && (*p != DELIMITER); p++) length++;
  
    return length;
}


/* 
 * Adds the next token in the arg str to an array and returns a pointer 
 * to the beginning of the strings next token or its null terminator. 
 * Toks should point to an empty position in the array.
 */
char* add_next_tok(char** toks, char* str) {
   
    // moves pointer to start of token if delimiter prefix exists
    while(*str == DELIMITER) str++;

    // copy the token to a new return string
    char* tok = (char*)calloc((tok_len(str)+1), sizeof(char));
    char* p;
    for(p=tok; *str && (*str != DELIMITER); str++, p++) *p=*str;

    // add the token
    *toks = tok;

    // move the pointer up to the start of the next token
    while(*str == DELIMITER) str++;

    return str;  
}


/* 
 * Tokenizes a string into an array of string pointers. These tokens are 
 * separated by the DELEMINATOR provided in zbell_prog1.h.
 */
char** argtok(char* str) {
    
    // +1 for the null terminator
    char** toks = (char**)calloc((n_tok(str)+1), sizeof(char*));
    
    char** tok_ptr = toks;
    while(*str) {
        str = add_next_tok(tok_ptr, str);
        tok_ptr++;
    };    
    
    return toks;
}

/* Frees the memory associated with an array of strings. */
void free_tokens(char** tokens) {
    char** tp = tokens;
    while(*tp) {
        char* tmp = *tp;
	tp++;
	free(tmp);
    }
    free(tokens);
}
