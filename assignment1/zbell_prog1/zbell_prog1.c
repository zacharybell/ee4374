#include <stdio.h>
#include <stdlib.h>
#include "zbell_argtok.h"

// buffer size for input
#define MAX 1000


/*
 * Used as a test driver to zbell_argtok.c/h
 */
int main() {

    // read input
    printf("$ ");
    char buffer[MAX];
    fgets(buffer, MAX, stdin);

    // tokenize the input string
    char** tokens = argtok(buffer);

    // print the array of tokens to stdout
    char** tok_ptr = tokens;
    while(*tok_ptr) {
	    printf("%s\n", *tok_ptr);
	    tok_ptr++;
    }

    // demonstrates memory deallocation (I know this is redundant)
    free_tokens(tokens);
	
    return 0;
}
