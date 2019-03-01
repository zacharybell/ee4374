#ifndef ZBELL_ARGTOK_H
#define ZBELL_ARGTOK_H


// delimiter used to parse tokens
const static char DELIMITER = ' '; 

/*
 * Creates a copy of a subset of tokens from the token array.
 *
 * Args:
 *      arg0 - the token array
 *      arg1 - the beginning of the subset (must be within bounds in args)
 *      arg2 - the end of the subset (must be within bounds and greater or equal to start
 *
 * Return:
 *      A copy of a subset of tokens
 */
char** sub_cpy(char**, int, int);

/* 
 * Tokenizes a string based on the delimiator specified by the DELIMITER variable.
 *
 * arg0: string to be tokenized
 * returns: an array of tokens 
 */
char** argtok(char*);

/*
 * Frees the memory given an array of tokens (or an array of strings).
 *
 * arg0: an array of strings
 * returns: void
 */
void free_tokens(char**);

#endif
