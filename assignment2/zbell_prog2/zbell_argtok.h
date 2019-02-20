#ifndef ZBELL_PROG1_H
#define ZBELL_PROG1_H


// delimiter used to parse tokens
const static char DELIMITER = ' '; 

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
